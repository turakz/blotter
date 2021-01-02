#ifndef _EVENTS_H
#define _EVENTS_H

#include <unordered_map>

namespace blotter {
    namespace events {
        class event_base {
        public:
            virtual ~event_base(){};
            virtual void Handle() = 0;
        };
        class event_manager_base {
        public:
            virtual ~event_manager_base(){};
            virtual void add_event(const std::string& which, event_base* event) = 0;
            virtual void remove_event(const std::string& which, event_base* event) = 0;
            virtual bool raise_event(const std::string& which) = 0;
        };
        class event_manager : public event_manager_base {
        public:
            ~event_manager();
            void add_event(const std::string& which, event_base* event);
            void remove_event(const std::string& which, event_base* event);
            bool raise_event(const std::string& event);
        private:
            std::unordered_map<std::string, event_base*> events_;
        };
    }//events
}//blotter
#endif
