#ifndef _STRATEGY
#define _STRATEGY
namespace blotter {
    class strategy {
    public:
        virtual ~strategy() {}
        virtual void execute() = 0;
    };
};
#endif
