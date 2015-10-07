#ifndef SERDA_JOYSTICK_H
#define SERDA_JOYSTICK_H

const std::map<sf::Joystick::Axis, std::string> JoyToStr{
    {sf::Joystick::Axis::X, "JoyX"},
    {sf::Joystick::Axis::Y, "JoyY"},
    {sf::Joystick::Axis::Z, "JoyZ"},
    {sf::Joystick::Axis::R, "JoyR"},
    {sf::Joystick::Axis::U, "JoyU"},
    {sf::Joystick::Axis::V, "JoyV"},
    {sf::Joystick::Axis::PovX, "JoyPX"},
    {sf::Joystick::Axis::PovY, "JoyPY"}};

#endif
