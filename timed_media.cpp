#include "timed_media.hpp"


void TimedText::init (Text *text, Timer *timer)
{
    this->text = text;
    this->timer = timer;
}

void TimedText::render (int x, int y, int w, int h, Text::TextAlign centering)
{
    if (timer->get_elapsed() <= time)
    {
        text->render(x, y, w, h, centering);
    }
}

void TimedText::set_text (const std::string &text, int time)
{
    this->text->set_text(text);
    timer->start();
    
    this->time = time;
}
