#ifndef _STRATEGY
#define _STRATEGY
class strategy {
public:
    virtual ~strategy() {}
    virtual void execute() = 0;
};
#endif
