#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "player.h"
#include "entity.h"
#include "floor.h"
#include "action.h"
#include "client.h"
#include "color.h"
#include "sidebar.h"
#include "message.h"

char msg[200];

entity *player;

void init_player() {
    player = create_entity(ET_CARROT);
}

void place_player() {
    
}

bool update_player() {
    struct rank_info rinfo;
    if(player->delay) {
        --(player->delay);
    }
    else {
        draw_sidebar();
        draw_message();
        
        switch(getch()) {
            case KEY_RIGHT:
                handle_player_enter_tile_event(get_tile_at(player->r, player->c+1));
                break;
            case KEY_LEFT:
                handle_player_enter_tile_event(get_tile_at(player->r, player->c-1));
                break;
            case KEY_UP:
                handle_player_enter_tile_event(get_tile_at(player->r-1, player->c));
                break;
            case KEY_DOWN:
                handle_player_enter_tile_event(get_tile_at(player->r+1, player->c));
                break;
            case '.':
                add_message("You waited");
                player->delay = 5000;
                break;
            case 'q':
                make_potatoboom();
                break;
            case 'w':
                call_peer();
                break;
            case 'e':
                throw_leaf();
                break;
            case 'i': case 'I':
                //인벤토리 창
                break;
            case '`':
                get_player()->hp = 10000;
                get_player()->power = 1000;
                get_player()->name = "SUPERCARROT";
                add_message("YOU ARE ALL MIGHTY!!!");

                return false;

            case 'p': case 'P':
                //종료(테스트용)
                rinfo = post_rank(get_tick());
                if(rinfo.prec_playtime==0){
                    fprintf(stderr,"server disconected\n");
                }
                sprintf(msg, "당신의 순위는 %d위(%d틱만에 클리어), 바로 앞 순위의 플레이어는 %d틱만에 클리어했어요!", rinfo.rank, get_tick(), rinfo.prec_playtime);
                close_connect();
                SET_COLOR(0);
                mvaddstr(0,1,msg);
                refresh();
                sleep(5);
                endwin();
                exit(0);
                break;
            case 'x':
                do_examine();
                return false;
            //...
            default:
                return false;
        }
    }
    return true;
}

//player entity의 포인터를 반환
entity *get_player(){
    return player;
}

void free_player() {
    free(player);
}

void make_potatoboom(){
    int row, col;
    entity *pl = get_player();
    if(pl->bombs > 0) {
        --(pl->bombs);
    }
    else {
        add_message("You ran out of potato bombs to throw.");
        return;
    }
    switch(getch()) {
    case KEY_RIGHT:
        row = player->r; col = player->c+1;
        break;
    case KEY_LEFT:
        row = player->r; col = player->c-1;
        break;
    case KEY_UP:
        row = player->r-1; col = player->c;
        break;
    case KEY_DOWN:
        row = player->r+1; col = player->c;
        break;
    default:
        make_potatoboom();
        return;
    }

    if(is_passable(row,col) && get_entity_at(row,col) == NULL){
        push_entity_into_room(NULL, create_entity(ET_POTATOBOOM), row, col, 1);

        add_message("You launch the potato bomb into the air, and it springs back to life.");
    }
    else
        make_potatoboom();

    player->delay = 7500;
}

void throw_leaf(){
    int row, col;
    entity *pl = get_player();

    if(pl->hp <= 10) {
        add_message("You don't have more leaves to throw...");
        return;
    }
    pl->hp -= 10;
    
    switch(getch()) {
    case KEY_RIGHT:
        row = player->r; col = player->c+1;
        break;
    case KEY_LEFT:
        row = player->r; col = player->c-1;
        break;
    case KEY_UP:
        row = player->r-1; col = player->c;
        break;
    case KEY_DOWN:
        row = player->r+1; col = player->c;
        break;
    default:
        throw_leaf();
        return;
    }

    if(is_passable(row,col) && get_entity_at(row,col) == NULL){
        push_entity_into_room(NULL, create_entity(ET_LEAF), row, col, 1);

        add_message("You throw your leaf. Recovery time must be needed.");
    }
    else
        throw_leaf();

    player->delay = 11000;
}

void call_peer(){
    int row, col;
    entity *e;
    entity *pl = get_player();
    if(pl->peers > 0) {
        --(pl->peers);
    }
    else {
        add_message("You ran out of dead eggplants to resurrect.");
        return;
    }
    switch(getch()) {
    case KEY_RIGHT:
        row = player->r; col = player->c+1;
        break;
    case KEY_LEFT:
        row = player->r; col = player->c-1;
        break;
    case KEY_UP:
        row = player->r-1; col = player->c;
        break;
    case KEY_DOWN:
        row = player->r+1; col = player->c;
        break;
    default:
        call_peer();
        return;
    }

    if(is_passable(row,col) && get_entity_at(row,col) == NULL){
        e = create_entity(ET_EGGPLANT);
        e->power = player->power / 2;
        e->hp = player->hp / 2;
        e->delay = player->delay;
        e->attack_de = player->attack_de;
        e->mv_de = player->mv_de;
        push_entity_into_room(NULL, e, row, col, 1);

        add_message("You bring back the dead eggplant to life. It now walks aimlessly.");
    }
    else
        call_peer();

    player->delay = 5000;
}