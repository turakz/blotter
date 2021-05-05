#ifndef _STRATEGY
#define _STRATEGY
namespace blotter {
    class strategy_base {
    public:
        strategy_base() = default;
        strategy_base(const strategy_base&) = delete;
        strategy_base& operator=(const strategy_base&) = delete;
        strategy_base(const strategy_base&&) = delete;
        strategy_base& operator=(const strategy_base&&) = delete;
        virtual ~strategy_base() {}
        virtual void execute() = 0;
    };
};
#endif
