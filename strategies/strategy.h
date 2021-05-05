#ifndef _STRATEGY
#define _STRATEGY
namespace blotter {
    class strategy_base {
    public:
        virtual ~strategy_base() {}
        virtual void execute() = 0;
    };
};
#endif
