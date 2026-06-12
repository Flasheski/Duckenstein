/*
** EPITECH PROJECT, 2025
** ui_buttons_events.c
** File description:
** Handle ui button events
*/

#include <SFML/Window/Event.h>
#include <SFML/Window/Mouse.h>

#include "graphics/engine.h"
#include "graphics/ui.h"

void ui_button_events(ui_button_t *button, const sfEvent *event,
    const engine_t *engine)
{
    if (!button || !event || !engine)
        return;
    if (event->type == sfEvtMouseButtonPressed
        && event->mouseButton.button == sfMouseLeft) {
        button->pressed = button->hovered;
    }
    if (event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseLeft) {
        if (button->on_click && button->hovered && button->pressed) {
            button->pressed = false;
            button->on_click(button->data);
            return;
        }
        button->pressed = false;
    }
}
