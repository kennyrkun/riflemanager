#ifndef SERIAL_INPUT_BOX_HPP
#define SERIAL_INPUT_BOX_HPP

#include <SFUI/Widget.hpp>
#include <SFUI/Utils/Box.hpp>

class SerialInputBox : public SFUI::Widget
{
public:
	SerialInputBox(float width = 200.f);

	void setText(const sf::String& string);

	bool isEmpty() const;

	const sf::String& getText() const;

	void setCursorPosition(size_t index);
	size_t getCursorPosition() const;

	void setBlinkPeriod(float period);
	float getBlinkPeriod() const;

protected:
	// Callbacks
	void onKeyPressed(sf::Keyboard::Key key);
	void onMousePressed(float x, float y);
	void onTextEntered(sf::Uint32 unicode);

	// FIXME: sometimes these keep the hovered state
	void onStateChanged(SFUI::State state);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	SFUI::Box         m_box;

	std::string m_oldText;
	sf::Text    m_text;

	mutable sf::RectangleShape m_cursor;
	mutable sf::Clock  m_cursor_timer;
	float			   m_blink_period;
	// TODO: add cursor delay (.25s)

	size_t m_max_characters;
	size_t m_cursor_pos;
};

#endif // !SERIAL_INPUT_BOX_HPP
