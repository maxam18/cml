//
//  btree16.h
//  filtron
//
//  Created by Max Amzarakov on 19.09.14.
//  Copyright (c) 2014 Uptime Technology. All rights reserved.
//

#ifndef filtron_btree16_h
#define filtron_btree16_h

#include <sys/types.h>
#include <stdint.h>

struct bt16_node_s {
	struct bt16_node_s *left;
	struct bt16_node_s *right;
	struct bt16_node_s *parent;
	enum { BT16_RED, BT16_BLACK } color;
	void           *data;
	int16_t           key;
};
typedef struct bt16_node_s bt16_node_t;

typedef struct {
	bt16_node_t *root;
    bt16_node_t *nul;
	void *userinfo;
} bt16_info_t;

bt16_info_t *bt16_create(bt16_info_t *info, bt16_node_t *root, void *user);

void bt16_insert(bt16_info_t *info, bt16_node_t *node);
void bt16_delete(bt16_info_t *info, bt16_node_t *node);
void bt16_turn_left(bt16_info_t *info, bt16_node_t *node);
void bt16_turn_right(bt16_info_t *info, bt16_node_t *node);
bt16_node_t *bt16_find(bt16_info_t *info, int16_t key);
void bt16_walk(bt16_info_t *info, bt16_node_t *node, void (*cmd)(void *data, void *userinfo));
bt16_node_t *bt16_max(bt16_info_t *info);
bt16_node_t *bt16_min(bt16_info_t *info);
/* protected */
void bt16_rb_norm(bt16_info_t *info, bt16_node_t *node);

#endif
