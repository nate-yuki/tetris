/**
 * @file  menu.cpp
 * @brief Implementation of the class Menu.
 */

#include "menu.hpp"
#include "util.hpp"


void Menu::init (
    Renderer &renderer, Font &font,
    const std::string &prompt, const std::vector<std::string> &options,
    const Color &promptFillColor, const Color &promptFrameColor,
    const Color &optionFillColor, const Color &optionFrameColor,
    const Color &selectedOptionFillColor, const Color &selectedOptionFrameColor,
    const Color &promptTextColor, const Color &optionTextColor
)
{
    // Create text for fitting
    unsigned maxLen = prompt.size();
    for (const std::string &option : options)
    {
        if (option.size() > maxLen)
        {
            maxLen = option.size();
        }
    }
    std::string maxText(maxLen, 'W');

    // Initialize boxes
    promptBox.init(
        renderer, font, prompt,
        promptFillColor, promptFrameColor, promptTextColor, maxText
    );
    optionBoxes.resize(options.size());
    for (unsigned i = 0; i < options.size(); ++i)
    {
        optionBoxes[i].init(
            renderer, font, options[i],
            optionFillColor, optionFrameColor, optionTextColor, maxText
        );
    }

    // Store option box colors so that they can be changed during selection
    this->selectedOptionFillColor = selectedOptionFillColor;
    this->selectedOptionFrameColor = selectedOptionFrameColor;
    this->optionFillColor = optionFillColor;
    this->optionFrameColor = optionFrameColor;

    // Set first option row as selected
    selectedInd = 0;
    choiceMade = false;
    optionBoxes[selectedInd].set_fill_color(selectedOptionFillColor);
    optionBoxes[selectedInd].set_frame_color(selectedOptionFrameColor);

    lastX = lastY = lastW = lastH = -1;
}

void Menu::free ()
{
    promptBox.free();
    for (TextBox &box: optionBoxes)
    {
        box.free();
    }
}

void Menu::handle_event (Game &game, const SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            // Paint the previous selected option box with the default color
            optionBoxes[selectedInd].set_fill_color(optionFillColor);
            optionBoxes[selectedInd].set_frame_color(optionFrameColor);
            // Wrap
            if (--selectedInd < 0)
            {
                selectedInd = optionBoxes.size() - 1;
            }
            // Paint the new selected option box with the selected option color
            optionBoxes[selectedInd].set_fill_color(selectedOptionFillColor);
            optionBoxes[selectedInd].set_frame_color(selectedOptionFrameColor);
            break;
            
        case SDLK_DOWN:
            // Paint the previous selected option box with the default color
            optionBoxes[selectedInd].set_fill_color(optionFillColor);
            optionBoxes[selectedInd].set_frame_color(optionFrameColor);
            // Wrap
            if (++selectedInd == optionBoxes.size())
            {
                selectedInd = 0;
            }
            // Paint the new selected option box with the selected option color
            optionBoxes[selectedInd].set_fill_color(selectedOptionFillColor);
            optionBoxes[selectedInd].set_frame_color(selectedOptionFrameColor);
            break;

        case SDLK_RETURN:
            choiceMade = true;
            break;
        }
    }
}

void Menu::render (int x, int y, int w, int maxH)
{
    int boxH = maxH / (1 + optionBoxes.size());
    int thickness = min(w, boxH) / 16;

    promptBox.render(x, y, w, boxH, thickness);
    for (unsigned i = 0; i < optionBoxes.size(); ++i)
    {
        optionBoxes[i].render(x, y + (i + 1) * boxH, w, boxH, thickness);
    }
}

int Menu::choosen_option () const
{
    if (choiceMade)
    {
        return selectedInd;
    }
    return -1;
}
