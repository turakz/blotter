#ifndef _EVENTS_H
#define _EVENTS_H

#include <boost/system/error_code.hpp>

#include <unordered_map>

namespace blotter {
    namespace events {
        class event_base {
        public:
            // we default construct derived classes on the heap
            event_base() = default;
            // because polymorphic, suppress copy, move to prevent slicing
            event_base(const event_base&) = delete;
            event_base& operator=(const event_base&) = delete;
            event_base(event_base&&) = delete;
            event_base& operator=(event_base&&) = delete;
            // we want classes managing derived events allocated via new to be able to call delete on their polymorphic representations
            // ie: Interface* obj = new Drived{}; ~Derived => for every derived stored as base, delete base end
            virtual ~event_base() = default;
            // polymorphic handling
            virtual void handler(boost::system::error_code& ec) = 0;
        };
        class event_manager_base {
        public:
            // we default construct derived classes statically
            event_manager_base() = default;
            // polymorphism/derived classes need to destroy resources -> suppress copy, move, implement derived destructor
            event_manager_base(const event_manager_base&) = delete;
            event_manager_base& operator=(const event_manager_base&) = delete;
            event_manager_base(event_manager_base&&) = delete;
            event_manager_base& operator=(event_manager_base&&) = delete;
            virtual ~event_manager_base(){};
            virtual void add_event(const std::string& which, event_base* event) = 0;
            virtual void remove_event(const std::string& which) = 0;
            // this interface is meant to be used in conjunction with boost facilities -- give event handlers access to boost error state
            virtual bool raise_event(const std::string& which, boost::system::error_code& ec) = 0;
        };
        class event_manager : public event_manager_base {
        public:
            event_manager() = default;
            event_manager(const event_manager&) = delete;
            event_manager& operator=(const event_manager&) = delete;
            event_manager(event_manager&&) = delete;
            event_manager& operator=(event_manager&&) = delete;
            ~event_manager() override;
            void add_event(const std::string& which, event_base* event) override;
            void remove_event(const std::string& which) override;
            bool raise_event(const std::string& event, boost::system::error_code& ec) override;
        private:
            std::unordered_map<std::string, event_base*> events_;
        };
    }//events
}//blotter
#endif
