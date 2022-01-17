// rwarrier
#ifndef STREAMSWITCHBOARD_H
#define STREAMSWITCHBOARD_H
#include <iostream>
#include <cstring>
#include "custom_fstream.h"

using namespace std;

class StreamSwitchboard {
    public:
        StreamSwitchboard(const char* search, const char* replace, const char* filename);
        StreamSwitchboard& get(char& c);
        StreamSwitchboard& put(char c);
        void completeMatch();
        void partialMatch(int matchCount, char mismatch);
        bool currentlyReplacing();
        bool eof() const;
    private:
        enum { CustFstream, SearchString, ReplacementString } getting_from;
        custom_fstream cfs;
        int remChar;
        int remCharPartial;
        int partialMatchLength;
        const char* searchString;
        const char* replaceString;
        bool eofFlag;
};

StreamSwitchboard::StreamSwitchboard(const char* search, const char* replace, const char* filename): searchString(search), replaceString(replace), cfs(filename), getting_from(CustFstream), remChar(0), remCharPartial(0), partialMatchLength(0), eofFlag(false) {

}

StreamSwitchboard& StreamSwitchboard::get(char& ch) {
    if(getting_from == ReplacementString) {
        ch = replaceString[strlen(replaceString) - remChar];
        remChar--;
        if(remChar == 0) {
            getting_from = CustFstream;
        }
    }
    else if(getting_from == SearchString) {
        ch = searchString[partialMatchLength - remCharPartial];
        remCharPartial--;
        if(remCharPartial == 0) {
            getting_from = CustFstream;
        }
    }
    else {
        cfs.get(ch);
    }
    return *this;
}

StreamSwitchboard& StreamSwitchboard::put(char c) {
    cfs.put(c);
    
    return *this;
}

void StreamSwitchboard::completeMatch() {
    getting_from = ReplacementString;
    remChar = strlen(replaceString);
}

void StreamSwitchboard::partialMatch(int matchCount, char mismatched_char) {
    cfs.pushToBufferFront(mismatched_char);
    cfs.put(searchString[0]);
    if(matchCount > 1) {
        getting_from = SearchString;
        partialMatchLength = matchCount;
        remCharPartial = matchCount - 1;
    }
}


bool StreamSwitchboard::currentlyReplacing() {
    return getting_from == ReplacementString;
}

bool StreamSwitchboard::eof() const{
    return cfs.endoffile();
}

#endif