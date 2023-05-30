#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "floor.h"
#include "util.h"

floor *cur_floor;

void init_floor() {
    cur_floor = malloc(sizeof(floor));
    memset(cur_floor, 0, sizeof(cur_floor));
    cur_floor->rooms = NULL;
    cvector_push_back(cur_floor->rooms, get_start_room());
    cvector_push_back(cur_floor->rooms, get_butcher_room());
    cvector_push_back(cur_floor->rooms, get_corridor_room());
    cvector_push_back(cur_floor->rooms, get_tmp_room());
    cur_floor->cur_room = cur_floor->rooms[0];

    link_rooms(cur_floor->rooms[0], cur_floor->rooms[2], 0, 0);

    //player를 room에 집어넣는다. 
    push_player_into_room(cur_floor->cur_room->roff + (cur_floor->cur_room->r-cur_floor->cur_room->roff)/2, cur_floor->cur_room->coff + (cur_floor->cur_room->c-cur_floor->cur_room->coff)/2);
    get_player()->bombs = 1;
    get_player()->peers = 1;
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


void link_rooms(room *a, room *b, int da, int db) {
    floor *f = cur_floor;
    int ra, rb;

    for(ra = 0; ra < cvector_size(f->rooms); ++ra) {
        if(f->rooms[ra] == a) break;
    }
    for(rb = 0; rb < cvector_size(f->rooms); ++rb) {
        if(f->rooms[rb] == b) break;
    }

    a->doors[da].next = b->doors[db].prev;
    b->doors[db].next = a->doors[da].prev;
    a->doors[da].room_id = rb;
    b->doors[db].room_id = ra;
}