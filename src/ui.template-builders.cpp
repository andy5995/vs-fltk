
#include <ui.hpp>
#include <ui-tree.hpp>

#include "FL/Fl_Box.H"
#include "FL/Fl_Widget.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Toggle_Button.H"
#include "FL/Fl_Input.H"

namespace vs{

    template class ui<Fl_Widget>;
    template class ui<Fl_Group>;
    template class ui<Fl_Window>;
    template class ui<Fl_Button>;
    template class ui<Fl_Toggle_Button>;
    template class ui<Fl_Input>;
    template class ui<Fl_Box>;

}