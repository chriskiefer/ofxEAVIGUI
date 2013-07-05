ofxEAVIGUI
==========

A GUI system for OpenFrameworks, for desktop and iOS


Features:
- high performance using FBOs
- the same code for iOS and desktop
- easily extendable with custom controls
- rotatable controls
- controls can follow screen rotation in iOS
- graphical effects can be added to any control
- handles multitouch, within single controls and across multiple controls



This project is currently under development, full documentation and a demo will follow.  

In brief, this is an example of some code for iOS

 --- in app.h

class GUIApp : public ofxiPhoneApp, public EAVIGUI::InterfaceListener {

...

 --- in app.mm

```c++

enum guiIDS {BUTTON1};

void setup() {

    button = new EAVIGUI::ImageButton(callback, BUTTON1, 0, 0, "button.png", "buttonOver.png"); //initialise a button
    button->setRelativePositioning(0.5, -button->getScaledWidth()/2.0, 0.5, -button->getScaledHeight()/2.0); //relative position, for screen rotations
    EAVIGUI::InterfaceManager::addObject(button); //add to gui
    button->setVisible(true);

    EAVIGUI::InterfaceManager::setup();

}

void handleInterfaceEvent(int intid, int eventTypeId, EAVIGUI::InterfaceObject *object) {
    cout << "Interface: " << intid << ", " << eventTypeId << endl;

    if (intid == BUTTON1) {
        switch(eventTypeId) {
            case EAVIGUI::InterfaceObject::TOUCHDOWN:
                //do something
                break;
            case EAVIGUI::InterfaceObject::TOUCHUP:
            case EAVIGUI::InterfaceObject::TOUCHEXIT:
                //do something else
                break;
        }
    }
}
```