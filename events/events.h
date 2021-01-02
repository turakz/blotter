#ifndef _EVENTS_H
#define _EVENTS_H

#include <boost/system/error_code.hpp>

#include <unordered_map>

namespace blotter {
    namespace events {
        class event_base {
        public:
            virtual ~event_base(){};
            virtual void handle(boost::system::error_code& ec) = 0;
        };
        class event_manager_base {
        public:
            virtual ~event_manager_base(){};
            virtual void add_event(const std::string& which, event_base* event) = 0;
            virtual void remove_event(const std::string& which, event_base* event) = 0;
            // this interface is meant to be used in conjunction with boost facilities -- give event handlers access to boost error state
            virtual bool raise_event(const std::string& which, boost::system::error_code& ec) = 0;
        };
        class event_manager : public event_manager_base {
        public:
            ~event_manager();
            void add_event(const std::string& which, event_base* event);
            void remove_event(const std::string& which, event_base* event);
            bool raise_event(const std::string& event, boost::system::error_code& ec);
        private:
            std::unordered_map<std::string, event_base*> events_;
        };
    }//events
}//blotter
#endif
