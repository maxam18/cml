/***************************************************************************
 *
 *                 btree.c  -  BTree implementation
 *                 ------------------------------------------
 *   begin         : óÒÄ ñÎ× 10 19:00:33 MSK 2007
 *   copyright     : Copyright (c) 2007 by Max Amzarakov
 *   email         : max _at_ stack-dot-net
 *   version       : $Id$
 ***************************************************************************/
/**
 * test on uid search for 4967650 records and 784557 uniq shows 50% performance gain against plain btree. 15 seconds for plain btree against 10 seconds for balanced tree.
 * use of -O2 optimization drops the perfomrance for 20%. (10 sec against 12 sec)
 */

#include <btree.h>
#include <stdlib.h>

static void bt_turn_left(bt_info_t *info, bt_node_t *node);
static void bt_turn_right(bt_info_t *info, bt_node_t *node);
static void bt_rb_norm(bt_info_t *info, bt_node_t *node);

bt_info_t *bt_create(bt_info_t *info, bt_node_t *root, int (*cmp)(const void *, const void *), void *data)
{
	info->root      = root;
	info->cmp       = cmp;
    info->userinfo  = data;
    info->nul       = calloc(1, sizeof(bt_node_t));
    root->color     = info->nul->color = BT_BLACK;
    root->left      = root->right = root->parent = info->nul;
 
	return info;
}

void bt_insert(bt_info_t *info, bt_node_t *node)
{
    bt_node_t *current, *next;
    
    current = next = info->root;
    
    node->color = BT_RED;
	node->right = node->left = info->nul;

	while( next != info->nul )
	{
        current = next;
		if( info->cmp(node->key, current->key) > 0 )
			next = current->right;
		else
			next = current->left;
	}
    
	if( info->cmp(node->key, current->key) > 0 )
		current->right = node;
	else
		current->left = node;

    node->parent = current;

	bt_rb_norm(info, node);

	return;
}

void bt_delete(bt_info_t *info, bt_node_t *node)
{
	bt_node_t *swapnode, *leftnode;

	if( node->left == info->nul )
	{
		swapnode = node->right;
	} else if ( node->right == info->nul )
	{
		swapnode = node->left;
	} else
	{
		swapnode = node->right;
		leftnode = swapnode;
		while( leftnode->left != info->nul )
		{
			leftnode = leftnode->left;
		}
		leftnode->left = node->left;
	}
	swapnode->parent = node->parent;
	if( node->parent != info->nul )
	{
		if( node->parent->left == node )
			node->parent->left = swapnode;
		else
			node->parent->right = swapnode;
	} else
		info->root = swapnode;
}

bt_node_t *bt_find(bt_info_t *info, void *key)
{
	bt_node_t *current = info->root;
	int found = 1;
	
	while( current != info->nul )
	{
		if( (found = info->cmp(key, current->key) ) == 0 )
		{
			break;
		}
		if( found > 0 )
			current = current->right;
		else
			current = current->left;
	}

	return found == 0 ? current : NULL;
}

void *bt_get(bt_info_t *info, void *key)
{
    bt_node_t *current = info->root;
    int found = 1;
    
    while( current != info->nul )
    {
        if( (found = info->cmp(key, current->key) ) == 0 )
        {
            break;
        }
        if( found > 0 )
            current = current->right;
        else
            current = current->left;
    }
    
    return found == 0 ? current->data : NULL;
}

void bt_walk(bt_info_t *info, bt_node_t *node, void (*cmd)(void *, void *))
{
	if( node != info->nul )
	{
        bt_walk(info, node->left , cmd);
		cmd(node->data, info->userinfo);
        bt_walk(info, node->right, cmd);
	}
}

void bt_walk_nr(bt_info_t *info, bt_node_t *node, void (*cmd)(void *, void*))
{
    bt_node_t *rstack, *fstack, *nodep, *cstack;
    
    nodep = node;
    
    /* use btree as stack. Left is the lower element, right is the pointer to the bt node (list data) */
    rstack = NULL;
    fstack = malloc(sizeof(*fstack));
    
    fstack->left = NULL;
    fstack->data = NULL;
    
    while( 1 )
    {
        if( nodep != info->nul )
        {
            /* put into stack */
            cstack = rstack;
            if( fstack->right )
            {
                rstack = fstack;
                fstack = fstack->right;
            } else
            {
                rstack = malloc(sizeof(*rstack));
            }

            // push node
            rstack->left  = cstack;
            rstack->right = nodep;
            
            // go left
            nodep = nodep->left;
        } else
        {
            if( rstack == NULL )
                break;
            
            /* pop node from the stack and keep element in the stack of free elements */
            nodep         = rstack->right;
            
            rstack->right = fstack;
            
            fstack        = rstack;
            rstack        = rstack->left;
            
            // print node
            cmd(nodep->data, info->userinfo);
            
            // go right
            nodep = nodep->right;
        }
    }
    
    /* clear memory */
    while( fstack )
    {
        cstack = fstack->right;
        free(fstack);
        fstack = cstack;
    }
}

static void bt_turn_right(bt_info_t *info, bt_node_t *node)/*{{{*/
{
	bt_node_t *left = node->left;

	if( left == info->nul )
		return;

	left->parent = node->parent;
	if( node->parent != info->nul )
	{
		if( node->parent->left == node )
			node->parent->left = left;
		else
			node->parent->right = left;
	} else
		info->root = left;
	node->left = left->right;
	node->left->parent = node;
	left->right = node;
	node->parent = left;
}/*}}}*/

static void bt_turn_left(bt_info_t *info, bt_node_t *node)/*{{{*/
{
	bt_node_t *right = node->right;

	if( right == info->nul )
		return;
	right->parent = node->parent;
	if( node->parent != info->nul )
	{
		if( node->parent->left == node )
			node->parent->left = right;
		else
			node->parent->right = right;
	} else
		info->root = right;
	node->right = right->left;
    node->right->parent = node;
	right->left = node;
	node->parent = right;
}/*}}}*/

static void bt_rb_norm(bt_info_t *info, bt_node_t *node)/*{{{*/
{
	bt_node_t *uncle;
	int left;

    /* node cannot be null at this point. node->parent == NULL means nul node */
    if( !node->parent || !node->parent->parent )
        return; /* no grandparent */

	if( node->parent->color == BT_BLACK )
		return; /* rb Ok! */
    
	/* if parent is red it is not a root, get uncle */
	if( node->parent->parent->left == node->parent )
	{
		uncle = node->parent->parent->right;
		left = 1;
	} else
	{
		uncle = node->parent->parent->left;
		left = 0;
	}
    if( !uncle )
        return; /* no uncle */
    
	if( uncle->color == BT_RED )
	{ /* uncle is red */
		uncle->parent->right->color = BT_BLACK; /* node's uncle */
		uncle->parent->left->color = BT_BLACK; /* node's parent */
        uncle->parent->color = BT_RED;
		bt_rb_norm(info, uncle->parent);
	} else
	{
		if( left && node->parent->right == node)
		{		
			bt_turn_left(info, node->parent);
			node = node->left;
		} else if( left == 0 && node->parent->left == node )
		{
			bt_turn_right(info, node->parent);
			node = node->right;
		}
		node->parent->color = BT_BLACK;
		node->parent->parent->color = BT_RED;
		if( left )
			bt_turn_right(info, node->parent->parent);
		else
			bt_turn_left(info, node->parent->parent);
	}

}/*}}}*/

bt_node_t *bt_max(bt_info_t *info)/*{{{*/
{
	bt_node_t *current = info->root;

	while( current->right != info->nul )
	{
		current = current->right;
	}
	return current;
}/*}}}*/

bt_node_t *bt_min(bt_info_t *info)/*{{{*/
{
	bt_node_t *current = info->root;

	while( current->left != info->nul )
	{
		current = current->left;
	}
	return current;
}/*}}}*/
