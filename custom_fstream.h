// rwarrier
#ifndef CUSTOM_FSTREAM_H
#define CUSTOM_FSTREAM_H
#include <iostream>
#include <fstream>
#include <deque>
#include <unistd.h>
#include "FStream.h"

using namespace std;

class custom_fstream : public FStream
{
private:
    const char *filename;
    deque<char> buffer;
    long get_position;
    long put_position;
    bool custom_eof;
    bool buffer_empty;

public:
    custom_fstream(const char *fname);
    ~custom_fstream();
    istream &get(char &c);
    ostream &put(const char &c);
    void pushToBufferFront(const char &c);
    bool endoffile() const;
};

custom_fstream::custom_fstream(const char *fname) : FStream(fname, ios::in | ios::out), get_position(0), put_position(0), custom_eof(0), buffer_empty(0), filename(fname) {}

custom_fstream::~custom_fstream()
{
    truncate(filename, put_position);
}

istream &custom_fstream::get(char &c)
{
    if(buffer.size()>0)
    {
    c=buffer.front();
    buffer.pop_front();
    return *this;
    }
    FStream::seekg(get_position);
    get_position=FStream::get(c).tellg();
    if(FStream::eof())
    {
        FStream::clear();
        custom_eof=true;
        buffer_empty=true;
    }
    return *this;
}

ostream &custom_fstream::put(const char &c)
{
    if(get_position==put_position && !custom_eof)
    {
        char ch;
        FStream::seekg(get_position);
        get_position=FStream::get(ch).tellg();
        if(FStream::eof())
        {
            FStream::clear();
            custom_eof=true;
        }
        if(!custom_eof)
            buffer.push_back(ch);
    }
    FStream::seekp(put_position);
    put_position=FStream::put(c).tellp();
    return *this;
}

void custom_fstream::pushToBufferFront(const char &c)
{
    buffer.push_front(c);
}

bool custom_fstream::endoffile() const
{
    return custom_eof && buffer_empty;
}

#endif