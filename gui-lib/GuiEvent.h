#ifndef GUIEVENT_H
#define GUIEVENT_H

#include <SFML\Window\Joystick.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Event.hpp>

namespace gui
{
	////////////////////////////////////////////////////////////
	/// Derived from sf::Event, data structure adapted to use in GUI.
	/// 
	///
	////////////////////////////////////////////////////////////
	class GuiEvent
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Size events parameters when a widget is resized (Resized)
		///
		////////////////////////////////////////////////////////////
		struct SizeEvent
		{
			unsigned int width;  ///< New width, in pixels
			unsigned int height; ///< New height, in pixels

			SizeEvent& operator=(const sf::Event::SizeEvent& other)
			{
				width = other.width;
				height = other.height;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Keyboard event parameters (KeyPressed, KeyReleased)
		///
		////////////////////////////////////////////////////////////
		struct KeyEvent
		{
			sf::Keyboard::Key code;    ///< Code of the key that has been pressed
			bool          alt;     ///< Is the Alt key pressed?
			bool          control; ///< Is the Control key pressed?
			bool          shift;   ///< Is the Shift key pressed?
			bool          system;  ///< Is the System key pressed?

			KeyEvent& operator=(const sf::Event::KeyEvent& other)
			{
				code = other.code;
				alt = other.alt;
				control = other.control;
				shift = other.shift;
				system = other.system;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Text event parameters (TextEntered)
		///
		////////////////////////////////////////////////////////////
		struct TextEvent
		{
			sf::Uint32 unicode; ///< UTF-32 Unicode value of the character

			TextEvent& operator=(const sf::Event::TextEvent& other)
			{
				unicode = other.unicode;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Mouse move event parameters (MouseMoved)
		///
		////////////////////////////////////////////////////////////
		struct MouseMoveEvent
		{
			int x; ///< X position of the mouse pointer, relative to the left of the owner window
			int y; ///< Y position of the mouse pointer, relative to the top of the owner window

			MouseMoveEvent& operator=(const sf::Event::MouseMoveEvent& other)
			{
				x = other.x;
				y = other.y;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Mouse drag event parameters (MouseDragBegin, MouseDrag, MouseDragEnd)
		///
		////////////////////////////////////////////////////////////
		struct MouseDragEvent
		{
			int x; ///< X position of the mouse pointer, relative to the left of the owner window
			int y; ///< Y position of the mouse pointer, relative to the top of the owner window

			MouseDragEvent& operator=(const sf::Event::MouseMoveEvent& other)
			{
				x = other.x;
				y = other.y;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Mouse buttons events parameters
		///        (MouseButtonPressed, MouseButtonReleased)
		///
		////////////////////////////////////////////////////////////
		struct MouseButtonEvent
		{
			sf::Mouse::Button button; ///< Code of the button that has been pressed
			int           x;      ///< X position of the mouse pointer, relative to the left of the owner window
			int           y;      ///< Y position of the mouse pointer, relative to the top of the owner window

			MouseButtonEvent& operator=(const sf::Event::MouseButtonEvent& other)
			{
				button = other.button;
				x = other.x;
				y = other.y;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Mouse buttons events parameters
		///        (MouseButtonPressed, MouseButtonReleased)
		///
		////////////////////////////////////////////////////////////
		struct MouseEnterLeftEvent
		{
			bool		  entered;///< True if mouse entered widget, false otherwise
			int           x;      ///< X position of the mouse pointer, relative to the left of the owner window
			int           y;      ///< Y position of the mouse pointer, relative to the top of the owner window
		};

		////////////////////////////////////////////////////////////
		/// \brief Mouse wheel events parameters (MouseWheelScrolled)
		///
		////////////////////////////////////////////////////////////
		struct MouseWheelScrollEvent
		{
			sf::Mouse::Wheel wheel; ///< Which wheel (for mice with multiple ones)
			float        delta; ///< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
			int          x;     ///< X position of the mouse pointer, relative to the left of the owner window
			int          y;     ///< Y position of the mouse pointer, relative to the top of the owner window

			MouseWheelScrollEvent& operator=(const sf::Event::MouseWheelScrollEvent& other)
			{
				wheel = other.wheel;
				delta = other.delta;
				x = other.x;
				y = other.y;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Joystick connection events parameters
		///        (JoystickConnected, JoystickDisconnected)
		///
		////////////////////////////////////////////////////////////
		struct JoystickConnectEvent
		{
			unsigned int joystickId; ///< Index of the joystick (in range [0 .. Joystick::Count - 1])

			JoystickConnectEvent& operator=(const sf::Event::JoystickConnectEvent& other)
			{
				joystickId = other.joystickId;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Joystick axis move event parameters (JoystickMoved)
		///
		////////////////////////////////////////////////////////////
		struct JoystickMoveEvent
		{
			unsigned int   joystickId; ///< Index of the joystick (in range [0 .. Joystick::Count - 1])
			sf::Joystick::Axis axis;       ///< Axis on which the joystick moved
			float          position;   ///< New position on the axis (in range [-100 .. 100])

			JoystickMoveEvent& operator=(const sf::Event::JoystickMoveEvent& other)
			{
				joystickId = other.joystickId;
				axis = other.axis;
				position = other.position;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Joystick buttons events parameters
		///        (JoystickButtonPressed, JoystickButtonReleased)
		///
		////////////////////////////////////////////////////////////
		struct JoystickButtonEvent
		{
			unsigned int joystickId; ///< Index of the joystick (in range [0 .. Joystick::Count - 1])
			unsigned int button;     ///< Index of the button that has been pressed (in range [0 .. Joystick::ButtonCount - 1])

			JoystickButtonEvent& operator=(const sf::Event::JoystickButtonEvent& other)
			{
				joystickId = other.joystickId;
				button = other.button;
				return *this;
			}
		};

		////////////////////////////////////////////////////////////
		/// \brief Enumeration of the different types of events
		///
		////////////////////////////////////////////////////////////
		enum EventType
		{
			Closed,                 ///< The window requested to be closed (no data)
			Resized,                ///< The window was resized (data in event.size)
			TextEntered,            ///< A character was entered (data in event.text)
			KeyPressed,             ///< A key was pressed (data in event.key)
			KeyReleased,            ///< A key was released (data in event.key)
			MouseWheelMoved,        ///< The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
			MouseWheelScrolled,     ///< The mouse wheel was scrolled (data in event.mouseWheelScroll)
			MouseButtonPressed,     ///< A mouse button was pressed (data in event.mouseButton)
			MouseButtonReleased,    ///< A mouse button was released (data in event.mouseButton)
			MouseMoved,             ///< The mouse cursor moved (data in event.mouseMove)
			JoystickButtonPressed,  ///< A joystick button was pressed (data in event.joystickButton)
			JoystickButtonReleased, ///< A joystick button was released (data in event.joystickButton)
			JoystickMoved,          ///< The joystick moved along an axis (data in event.joystickMove)
			JoystickConnected,      ///< A joystick was connected (data in event.joystickConnect)
			JoystickDisconnected,   ///< A joystick was disconnected (data in event.joystickConnect)

			// Non recursive events
			LostFocus,              ///< The focused widget lost the focus (no data)
			GainedFocus,            ///< The widget gained the focus (no data)
			MouseEntered,           ///< The mouse cursor entered the widget (data in event.MouseEnterLeftEvent)
			MouseLeft,              ///< The mouse cursor left the widget (data in event.MouseEnterLeftEvent)
			MouseDragBegin,			///< The widget is starting to being dragged
			MouseDrag,				///< The widget is being dragged
			MouseDragEnd,			///< The widget stopped being dragged

			Count                   ///< The total number of event types
		};

		////////////////////////////////////////////////////////////
		// Member data
		////////////////////////////////////////////////////////////
		EventType type; ///< Type of the event

		union
		{
			SizeEvent             size;              ///< Size event parameters (GuiEvent::Resized)
			KeyEvent              key;               ///< Key event parameters (GuiEvent::KeyPressed, GuiEvent::KeyReleased)
			TextEvent             text;              ///< Text event parameters (GuiEvent::TextEntered)
			MouseMoveEvent        mouseMove;         ///< Mouse move event parameters (GuiEvent::MouseMoved)
			MouseDragEvent		  mouseDrag;		 ///< Mouse drag event parameters (GuiEvent::MouseDragBegin, GuiEvent::MouseDrag, GuiEvent::MouseDragEnd)
			MouseEnterLeftEvent	  mouseEnterLeft;	 ///< Mouse entered or left the widget(GuiEvent::MouseEntered, GuiEvent::MouseLeft)
			MouseButtonEvent      mouseButton;       ///< Mouse button event parameters (GuiEvent::MouseButtonPressed, GuiEvent::MouseButtonReleased)
			//MouseWheelEvent       mouseWheel;        ///< Mouse wheel event parameters (GuiEvent::MouseWheelMoved) (deprecated)
			MouseWheelScrollEvent mouseWheelScroll;  ///< Mouse wheel event parameters (GuiEvent::MouseWheelScrolled)
			JoystickMoveEvent     joystickMove;      ///< Joystick move event parameters (GuiEvent::JoystickMoved)
			JoystickButtonEvent   joystickButton;    ///< Joystick button event parameters (GuiEvent::JoystickButtonPressed, GuiEvent::JoystickButtonReleased)
			JoystickConnectEvent  joystickConnect;   ///< Joystick (dis)connect event parameters (GuiEvent::JoystickConnected, GuiEvent::JoystickDisconnected)
		};
	};
}
#endif