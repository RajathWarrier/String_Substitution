// rwarrier
#include <iostream>
#include <fstream>
#include <cstring>
#include "streamswitchboard.h"

using namespace std;

int main(int argc, char *argv[])
{
    const char *searchWord = argv[1];
    StreamSwitchboard ss(argv[1], argv[2], argv[3]);
    char c;
    int matchCount = 0;
    while (!ss.get(c).eof())
    {
        if (!ss.currentlyReplacing() && c == searchWord[matchCount])
        {
            ++matchCount;
            if (matchCount < strlen(searchWord))
            {
                continue;
            }
        }
        if (matchCount == strlen(searchWord))
        {
            ss.completeMatch();
        }
        else if (matchCount > 0)
        {
            ss.partialMatch(matchCount, c);
        }
        else
        {
            ss.put(c);
        }
        matchCount = 0;
    }
    return 0;
}
