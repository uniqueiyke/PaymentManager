#ifndef GWARGUMENTS_H
#define GWARGUMENTS_H


class GwArguments
{
public:
    GwArguments(int argc = 0, char **argv = nullptr);
    void setArgc(int argc) { argc_= argc; }
    void setArgv(char **argv) { argv_= argv; }

    int argc(){ return argc_; }
    char **argv(){ return argv_; }

private:
    int argc_;
    char **argv_;
};

#endif // GWARGUMENTS_H
