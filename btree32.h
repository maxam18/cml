//
//  btree32.h
//  filtron
//
//  Created by Max Amzarakov on 19.09.14.
//  Copyright (c) 2014 Uptime Technology. All rights reserved.
//

#ifndef filtron_btree32_h
#define filtron_btree32_h

#include <sys/types.h>
#include <stdint.h>

struct bt32_node_s {
	struct bt32_node_s *left;
	struct bt32_node_s *right;
	struct bt32_node_s *parent;
	enum { BT32_RED, BT32_BLACK } color;
	void           *data;
	int32_t           key;
};
typedef struct bt32_node_s bt32_node_t;

typedef struct {
	bt32_node_t *root;
    bt32_node_t *nul;
	void *userinfo;
} bt32_info_t;

bt32_info_t *bt32_create(bt32_info_t *info, bt32_node_t *root, void *user);

void bt32_insert(bt32_info_t *info, bt32_node_t *node);
void bt32_delete(bt32_info_t *info, bt32_node_t *node);
void bt32_turn_left(bt32_info_t *info, bt32_node_t *node);
void bt32_turn_right(bt32_info_t *info, bt32_node_t *node);
bt32_node_t *bt32_find(bt32_info_t *info, int32_t key);
void *bt32_get(bt32_info_t *info, int32_t key);
void bt32_walk(bt32_info_t *info, bt32_node_t *node, void (*cmd)(void *data, void *userinfo));
void bt32_traverse(bt32_info_t *info, void (*cmd)(void *data, void *userinfo));

bt32_node_t *bt32_max(bt32_info_t *info);
bt32_node_t *bt32_min(bt32_info_t *info);
/* protected */
void bt32_rb_norm(bt32_info_t *info, bt32_node_t *node);

#endif
