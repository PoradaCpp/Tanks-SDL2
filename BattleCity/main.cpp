#include <iostream>
#include <string>
#include <algorithm>

#include "state.h"
#include "buttonnamed.h"
#include "startpage.h"
#include "createmappage.h"
#include "gamepage.h"
#include "startpageinitdata.h"
#include "createmappageinitdata.h"
#include "gamepageinitdata.h"

#ifdef __MINGW32__
#undef main        // Prevents SDL from overriding main()
#endif

int main()
{
    State MainState;

    {
        MainWindow window( "BATTLE CITY", "media/images/win_icon.png" );

        StartPageInitData     startPageInitData    ( window.getRenderer() );
        CreateMapPageInitData createMapPageInitData( window.getRenderer() );
        GamePageInitData      gamePageInitData     ( window.getRenderer() );

        window.attachPage( std::make_shared <StartPage>     ( startPageInitData,     &MainState                       ));
        window.attachPage( std::make_shared <CreateMapPage> ( createMapPageInitData, &MainState, window.getRenderer() ));
        window.attachPage( std::make_shared <GamePage>      ( gamePageInitData, gamePageInitData, &MainState, window.getRenderer() ));
        MainState.attachWindow( window );
    }

    MainState.mainAction();

    return 0;
}
