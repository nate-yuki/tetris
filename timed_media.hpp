#ifndef TIMED_MEDIA_HPP
#define TIMED_MEDIA_HPP


#include "text.hpp"
#include "timer.hpp"

#include <string>


class TimedText
{
public:
    void init(Text *text, Timer *timer);
    void render(
        int x, int y, int w, int h,
        Text::TextAlign centering=Text::TextAlign::TEXT_CENTER
    );
    void set_text(const std::string &text, int time, const Color *color=nullptr);

private:
    Text *text;
    Timer *timer;
    int time;
};


#endif
