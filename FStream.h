//use this class instead of fstream
#ifndef FSTREAM_H
#define FSTREAM_H
#include<cassert>
#include<fstream>
class FStream : public std::fstream {
  public:
   FStream(const std::string& fname,  std::ios_base::openmode mode)
     : std::fstream(fname, mode)
   {
     assert(std::fstream::good());
   }

   std::ostream& seekp(std::streampos newpos)
    {
      if(newpos==-1) return *this;
      assert(newpos>=putpos);
      putpos=newpos;
      return std::ostream::seekp(newpos);
    }

    std::istream& seekg(std::streampos newpos)
    {
      if(newpos==-1) return *this;
      assert(newpos>=getpos);
      getpos=newpos;
      return std::istream::seekg(newpos);
    }
  private:
    std::streampos putpos{0};
    std::streampos getpos{0};
};
#endif
