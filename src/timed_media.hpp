/**
 * @file  timed_media.hpp
 * @brief Include file for timed media classes.
 */

#ifndef TIMED_MEDIA_HPP
#define TIMED_MEDIA_HPP


#include "text.hpp"
#include "timer.hpp"

#include <string>


/// Text that stops rendering after set time.
class TimedText
{
public:
    /**
     * @brief Pass the underlying objects.
     * @param text Text to display. Should be initialized independently.
     * @param timer Timer to use.
     */
    void init(Text *text, Timer *timer);

    /**
     * @brief Render the current text if there is still time left since the last
     *     `set_text` call.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w Maximum width.
     * @param h Maximum height.
     * @param centering Position to place the text in relative to the rectangle.
     */
    void render(
        int x, int y, int w, int h,
        Text::TextAlign centering=Text::TextAlign::TEXT_CENTER
    );

    /**
     * @brief Set new text and start the timer.
     * @param text The new text.
     * @param time Time before the text stops being rendered.
     * @param color New color; by default, does not change collor.
     */
    void set_text(const std::string &text, int time, const Color *color=nullptr);

private:
    Text *text;
    Timer *timer;
    int time;
};


#endif
