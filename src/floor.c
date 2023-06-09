#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "floor.h"
#include "util.h"
#include "message.h"

floor *cur_floor;

void init_floor() {
    cur_floor = malloc(sizeof(floor));
    memset(cur_floor, 0, sizeof(cur_floor));
    cur_floor->rooms = NULL;
    cvector_push_back(cur_floor->rooms, get_start_room());
    cvector_push_back(cur_floor->rooms, get_butcher_room());
    cvector_push_back(cur_floor->rooms, get_corridor_room());
    cvector_push_back(cur_floor->rooms, get_tmp_room());
    cvector_push_back(cur_floor->rooms, get_storage_room());
    cvector_push_back(cur_floor->rooms, get_passageway());
    cvector_push_back(cur_floor->rooms, get_ambush_room());
    cvector_push_back(cur_floor->rooms, get_boss_room());
    cur_floor->cur_room = cur_floor->rooms[0];

    link_rooms(0, 4, 0, 0);
    link_rooms(4, 5, 1, 0);
    link_rooms(5, 1, 1, 0);
    link_rooms(1, 6, 1, 0);
    link_rooms(6, 2, 1, 0);
    link_rooms(2, 7, 1, 0);

    //player를 room에 집어넣는다. 
    push_player_into_room(cur_floor->cur_room->roff + (cur_floor->cur_room->r-cur_floor->cur_room->roff)/2, cur_floor->cur_room->coff + (cur_floor->cur_room->c-cur_floor->cur_room->coff)/2+1);
    get_player()->bombs = 0;
    get_player()->peers = 0;

    cur_floor->cur_room->check = false;
    add_message("You arise from a pool of blood...");
    add_message("You find yourself in the %s.", cur_floor->cur_room->name);
    add_message(cur_floor->cur_room->desc);
}
void draw() {
    if(cur_floor->room_changed) {
        cur_floor->room_changed = false;
        clear();
    }
    draw_room(cur_floor->cur_room);
    refresh();
}
void update(floor *f) {

}

//현재 방의 포인터를 반환
room *get_cur_room(){
    return cur_floor->cur_room;
}
floor *get_cur_floor() {
    return cur_floor;
}

void free_floor() {
    floor *f = cur_floor;
    int i;
    for(i = 0; i < cvector_size(f->rooms); ++i) free_room(f->rooms[i]);
    cvector_free(f->rooms);
}

int get_room_index(room *rm) {
    floor *f = cur_floor;
    int i;

    for(i = 0; i < cvector_size(f->rooms); ++i) {
        if(f->rooms[i] == rm) return i;
    }
    return -1;
}


void link_rooms(int ra, int rb, int da, int db) {
    floor *f = cur_floor;
    room *a, *b;

    a = f->rooms[ra];
    b = f->rooms[rb];

    a->doors[da].next = b->doors[db].prev;
    b->doors[db].next = a->doors[da].prev;
    a->doors[da].room_id = rb;
    b->doors[db].room_id = ra;
}