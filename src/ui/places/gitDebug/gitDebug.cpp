#include "gitDebug.h"

#if DEBUG_MENUS

void initGitDebug()
{
    init_general_page(6);
    general_page_set_title(DEBUG_MENU_GIT);
    genpage_set_center();
  
    genpage_add(DEBUG_GIT_COMMIT_HASH);
    genpage_add(GIT_COMMIT_HASH);

    genpage_add(DEBUG_GIT_BRANCH);
    genpage_add(GIT_BRANCH);

    genpage_add(DEBUG_GIT_BUILD_TIME);
    genpage_add(BUILD_TIME);

    general_page_set_main();    
}

#endif