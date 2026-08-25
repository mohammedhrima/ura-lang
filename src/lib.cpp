
class File
{
private:
    std::string filename;
    std::string _content;
    bool failed;

public:
    File(std::string filename) : filename(filename)
    {
        std::ifstream in;
        in.open(filename);
        if ((failed = !in.is_open()))
            return;
        in.seekg(0, std::ios::end);
        size_t len = in.tellg();
        in.seekg(0, std::ios::beg);
        _content.resize(len);
        in.read(_content.data(), len);
        in.close();
    };

    bool is_failed() { return failed; }
    std::string &content() { return _content; };
    size_t length() { return _content.length(); }

    ~File()
    {
    }
};