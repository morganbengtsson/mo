#include "stream.hpp"

namespace mo {

Stream::Stream(const std::string file_name):
file_name_(file_name), buffer_size(4096*8){
    vorbis_stream_ = stb_vorbis_open_filename((char*)file_name.c_str(), NULL, NULL);
    vorbis_info_ = stb_vorbis_get_info(vorbis_stream_);
    samples_left_ = 	stb_vorbis_stream_length_in_samples(vorbis_stream_) * vorbis_info_.channels;
}

Stream::Stream(const Stream &stream) : Stream(stream.file_name_) {
}

Stream::~Stream() {
    stb_vorbis_close(vorbis_stream_);
}

std::array<short, 4096*8> Stream::read(){
    auto samples = std::array<short, 4096*8>();

    int  size = 0;
    int  result = 0;

    while(size < buffer_size){
        result = stb_vorbis_get_samples_short_interleaved(vorbis_stream_, vorbis_info_.channels, samples.data()+size, buffer_size-size);
        if(result > 0) size += result*vorbis_info_.channels;
        else break;
    }
    samples_left_ -= size;
    return samples;
}

bool Stream::done() const{
    return samples_left_ <= 0 ? true : false;
}

int Stream::sample_rate() const
{
    return vorbis_info_.sample_rate;
}

int Stream::channels() const
{
    return vorbis_info_.channels;
}

void Stream::seek_start()
{
    stb_vorbis_seek_start(vorbis_stream_);
    samples_left_ = stb_vorbis_stream_length_in_samples(vorbis_stream_) * channels();
}

}
