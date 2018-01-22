/***************************************************************************
 *
 *                 btree.h  - Balanced Binary Tree implementation
 *                 ------------------------------------------
 *   begin         : óÒÄ ñÎ× 10 19:00:28 MSK 2007
 *   copyright     : Copyright (c) 2007 by Max Amzarakov
 *   email         : max _at_ stack-dot-net
 *   version       : $Id$
 ***************************************************************************/

#ifndef BTREE_H
#define BTREE_H

struct bt_node_s {
	struct bt_node_s *left;
	struct bt_node_s *right;
	struct bt_node_s *parent;
	enum { BT_RED, BT_BLACK } color;
	void *data;
	void *key;
};
typedef struct bt_node_s bt_node_t;

struct bt_info_s {
	bt_node_t *root;
    bt_node_t *nul;
	int (*cmp)(const void *, const void*);
	void *userinfo;
};
typedef struct bt_info_s bt_info_t;

/**
 \brief set up new btree structure
 \param info - btree structure pointer
 \param root - root node pointer 
 \param cmp  - comparision function pointer
 \param user - user data pointer (can be used for walk procedure)
 \return pointer to initialized struct (same as param info)
 */
bt_info_t *bt_create(bt_info_t *info, bt_node_t *root, int (*cmp)(const void*, const void *), void *user);

void bt_insert(bt_info_t *info, bt_node_t *node);
void bt_delete(bt_info_t *info, bt_node_t *node);
/**
 \brief lookup for key in the store
 \return bt_node_t pointer or NULL
 */
bt_node_t *bt_find(bt_info_t *info, void *key);
/**
 \brief lookup for key in the store 
 \return bt_node_t->data pointer or NULL
 */
void *bt_get(bt_info_t *info, void *key);

void bt_walk(bt_info_t *info, bt_node_t *node, void (*cmd)(void *data, void *userinfo));
void bt_walk_nr(bt_info_t *info, bt_node_t *node, void (*cmd)(void *data, void *userinfo));

bt_node_t *bt_max(bt_info_t *info);
bt_node_t *bt_min(bt_info_t *info);
/* protected */

#endif

