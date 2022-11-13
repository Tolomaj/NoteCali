/* eventslist ;
BUTTON_CLICK = 0,              // click on button
BUTTON_PRESS = 1,              // mouse down or key down in button
BUTTON_STATE_CHANGED = 2,      // checkbox/radio/slider changed its state/value
EDIT_VALUE_CHANGING = 3,       // before text change
EDIT_VALUE_CHANGED = 4,        // after text change
SELECT_SELECTION_CHANGED = 5,  // selection in <select> changed
SELECT_STATE_CHANGED = 6,      // node in select expanded/collapsed, heTarget is the node
POPUP_REQUEST = 7,           // request to show popup just received,                                //     here DOM of popup element can be modifed.
POPUP_READY = 8,           // popup element has been measured and ready to be shown on screen,
POPUP_DISMISSED = 9,           // popup element is closed,
MENU_ITEM_ACTIVE = 0xA,        // menu item activated by mouse hover or by keyboard,
MENU_ITEM_CLICK = 0xB,         // menu item click,
CONTEXT_MENU_REQUEST = 0x10,   // "right-click", BEHAVIOR_EVENT_PARAMS::he is current popup menu HELEMENT being processed or NULL.
VISIUAL_STATUS_CHANGED = 0x11, // broadcast notification, sent to all elements of some container being shown or hidden
DISABLED_STATUS_CHANGED = 0x12,// broadcast notification, sent to all elements of some container that got new value of :disabled state
POPUP_DISMISSING = 0x13,       // popup is about to be closed
CONTENT_CHANGED = 0x15,        // content has been changed, is posted to the element that gets content changed,  reason is combination of CONTENT_CHANGE_BITS.
CLICK = 0x16,                  // generic click
CHANGE = 0x17,                 // generic change
HYPERLINK_CLICK = 0x80,        // hyperlink click
ELEMENT_COLLAPSED = 0x90,      // element was collapsed, so far only behavior:tabs is sending these two to the panels
ELEMENT_EXPANDED,              // element was expanded,
ACTIVATE_CHILD,                // activate (select) child,
INIT_DATA_VIEW,                // request to virtual grid to initialize its view
ROWS_DATA_REQUEST,             // request from virtual grid to data source behavior to fill data in the table
UI_STATE_CHANGED,              // ui state changed, observers shall update their visual states.
FORM_SUBMIT,                   // behavior:form detected submission event. BEHAVIOR_EVENT_PARAMS::data field contains data to be posted.
FORM_RESET,                    // behavior:form detected reset event (from button type=reset). BEHAVIOR_EVENT_PARAMS::data field contains data to be reset.
DOCUMENT_COMPLETE,             // document in behavior:frame or root document is complete.
HISTORY_PUSH,                  // requests to behavior:history (commands)
HISTORY_DROP,
HISTORY_PRIOR,
HISTORY_NEXT,
HISTORY_STATE_CHANGED,         // behavior:history notification - history stack has changed
CLOSE_POPUP,                   // close popup request,
REQUEST_TOOLTIP,               // request tooltip, evt.source <- is the tooltip element.
ANIMATION = 0xA0,      // animation started (reason=1) or ended(reason=0) on the element.
DOCUMENT_CREATED = 0xC0,      // document created, script namespace initialized. target -> the document
DOCUMENT_CLOSE_REQUEST = 0xC1, // document is about to be closed, to cancel closing do: evt.data = sciter::value("cancel");
DOCUMENT_CLOSE = 0xC2,      // last notification before document removal from the DOM
DOCUMENT_READY = 0xC3,      // document has got DOM structure, styles and behaviors of DOM elements. Script loading run is complete at this moment. 
VIDEO_INITIALIZED = 0xD1,      // <video> "ready" notification   
VIDEO_STARTED = 0xD2,      // <video> playback started notification   
VIDEO_STOPPED = 0xD3,      // <video> playback stoped/paused notification   
VIDEO_BIND_RQ = 0xD4,      // <video> request for frame source binding, 
PAGINATION_STARTS = 0xE0,     // behavior:pager starts pagination
PAGINATION_PAGE = 0xE1,     // behavior:pager paginated page no, reason -> page no
PAGINATION_ENDS = 0xE2,     // behavior:pager end pagination, reason -> total pages
FIRST_APPLICATION_EVENT_CODE = 0x100
*/  // events list



class SettingsWin : public sciter::window {
public:
    SettingsWin() : window(SW_POPUP | SW_ENABLE_DEBUG | SW_HIDE, { 0, 0 , 600, 500 }) {}

    SOM_PASSPORT_BEGIN(SettingsWin)
        SOM_FUNCS(
            SOM_FUNC(nativeMessage)
        )SOM_PASSPORT_END

        

        virtual bool handle_event(HELEMENT, BEHAVIOR_EVENT_PARAMS& params) { 
        sciter::dom::element target = params.heTarget;
       debugLOG("something with: " + intToHEXstr(params.cmd) + " - " + std::to_string(params.cmd)  + " - " + WstrToStr(params.name) + " - " + std::to_string(params.reason));



        switch (params.cmd) {
        case CHANGE:
         // debugLOG("something changes");
            break;

        case 161: // not in doc caled twice
            if (target.test("switch.inp") && params.reason == 0) {
                sciter::dom::element targetP = target.parent();
                debugLOG(L"hvent catch with 161:" + std::to_wstring(params.cmd) + L" - " + targetP.get_attribute("id") + L" - " + std::to_wstring(target.get_value().get(0)) + L" -daps: " + std::to_wstring(params.reason) );
                settings.setSingleSettingsbyName(sciterStrToStr(targetP.get_attribute("id")), (bool)target.get_value().get(0));
                settings.saveSettings();
                return true; // handled
            }

            if (target.test("caption") && params.reason == 0) {
                sciter::dom::element targetP = target.parent();
                targetP = targetP.parent();
                targetP = targetP.parent();
                debugLOG(L"hvent catch with 161:" + std::to_wstring(params.cmd) + L" - " + targetP.get_attribute("id") + L" - " + std::to_wstring((double)target.get_value().get<double>()) + L" " + target.get_value().to_string() + L" -daps: " + std::to_wstring(params.reason));
                
                return true; // handled
            }
            break;
        case BUTTON_CLICK:
            sciter::string elementId = target.get_attribute("id");
            
           /* if (target.test("switch.inp")) {
                sciter::dom::element targetP = target.parent();
                
                debugLOG(L"hvent:" + std::to_wstring(params.cmd) + L" - " + targetP.get_attribute("id") + L" - " + std::to_wstring(target.get_value().get(0)));
                return true; // handled
            }*/

            if (target.test("button#styleBtn")) {
                // click on <button id="foo"> ...
                // for now:
                //target.attach_event_handler(&myHandler);
                debugLOG("btncls");
                return true; // handled
            }
        }
        return false;
    }

        sciter::string  nativeMessage() { return L"Hello C++ World"; }

};