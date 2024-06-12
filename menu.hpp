/**
 * @file  menu.hpp
 * @brief Include file for Menu class.
 */

#ifndef MENU_HPP
#define MENU_HPP


#include "renderer.hpp"
#include "font.hpp"
#include "textbox.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <vector>


/**
 * @brief A multiple row menu with a prompt at the top allowing for a single option
 *     choice.
 */
class Menu
{
public:
    /**
     * @brief Initialize class members.
     * @details
     * The longest text length is used for font fitting.
     * First row is set as the default selected one.
     * @param renderer Renderer to use for text rendering.
     * @param font Font to use for text.
     * @param prompt Prompt text.
     * @param options Option texts in order.
     * @param promptFillColor Fill color for the prompt box.
     * @param promptFrameColor Frame color for the prompt box.
     * @param optionFillColor Fill color for unselected option boxes.
     * @param optionFrameColor Frame color for unselected option boxes.
     * @param selectedOptionFillColor Fill color for the selected option box.
     * @param selectedOptionFrameColor Frame color for the selected option box.
     * @param promptTextColor Prompt text color.
     * @param optionTextColor Option text color.
     */
    void init(
        Renderer &renderer, Font &font,
        const std::string &prompt, const std::vector<std::string> &options,
        const Color &promptFillColor={128, 128, 128},
        const Color &promptFrameColor={0, 0, 0},
        const Color &optionFillColor={192, 192, 192},
        const Color &optionFrameColor={64, 64, 64},
        const Color &selectedOptionFillColor={224, 224, 224},
        const Color &selectedOptionFrameColor={255, 255, 255},
        const Color &promptTextColor={0, 0, 0}, const Color &optionTextColor={0, 0, 0}
    );

    /// Free the boxes.
    void free();
    
    /**
     * @brief Handle SDL events.
     * @details
     * Scrolls the selected option up or down with wrapping on UP or DOWN key presses
     * respectively. Sets the current selected option as chosen on Enter key press.
     */
    void handle_event(Game &game, const SDL_Event &e);

    /**
     * @brief Draw the menu with given location and dimensions.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w The width.
     * @param maxH Maximum height; rows are set to even height so the menu might turn
     *     out shorter.
     */
    void render(int x, int y, int w, int maxH);

    /// Get the chosen option index or `-1` if the choice was not made yet.
    int choosen_option() const;

private:
    TextBox promptBox;
    std::vector<TextBox> optionBoxes;
    Color selectedOptionFillColor, selectedOptionFrameColor;
    Color optionFillColor, optionFrameColor;
    int selectedInd; // Current selected option.
    bool choiceMade; // True if the selection was made (Enter pressed).

    int lastX, lastY, lastW, lastH;
};


#endif
