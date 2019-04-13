#include <iostream>
#include <string>
#include <algorithm>

#include "state.h"
#include "button.h"
#include "startpage.h"
#include"createmappage.h"
#include "startpageinitdata.h"
#include "createmappageinitdata.h"

#ifdef __MINGW32__
#undef main        // Prevents SDL from overriding main()
#endif

int main()
{
    State MainState;

    {
        MainWindow window( "BATTLE CITY", "media/images/win_icon.png" );

        StartPageInitData     startPageInitData( window.getRenderer() );
        CreateMapPageInitData createMapPageInitData( window.getRenderer() );

        window.attachPage( std::make_shared < StartPage >    ( startPageInitData ));
        window.attachPage( std::make_shared < CreateMapPage > ( createMapPageInitData, window.getRenderer() ));
        MainState.attachWindow( window );
    }

    MainState.mainAction();

    return 0;
}
