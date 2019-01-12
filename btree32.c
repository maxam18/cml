//
//  btree32.c
//  Red/Black balanced tree with int32 key
//
//  Created by Max Amzarakov on 19.09.14.
//  Copyright (c) 2014 Uptime Technology. All rights reserved.
//

#include <btree32.h>
#include <stdlib.h>

bt32_info_t *bt32_create(bt32_info_t *info, bt32_node_t *root, void *data)
{
    info->root      = root;
    info->userinfo  = data;
    info->nul       = calloc(1, sizeof(bt32_node_t));
    root->color     = info->nul->color = BT32_BLACK;
    root->left      = root->right = root->parent = info->nul;
 
    return info;
}

void bt32_insert(bt32_info_t *info, bt32_node_t *node)
{
    bt32_node_t *current, *next;
    
    current = next = info->root;
    
    node->color = BT32_RED;
    node->right = node->left = info->nul;

    while( next != info->nul )
    {
        current = next;
        
        if( node->key > current->key )
            next = current->right;
        else
            next = current->left;
    }
    
    if( node->key > current->key )
        current->right = node;
    else
        current->left = node;

    node->parent = current;

    bt32_rb_norm(info, node);

    return;
}

void bt32_delete(bt32_info_t *info, bt32_node_t *node)
{
    bt32_node_t *swapnode, *leftnode;

    if( node->left == info->nul )
    {
        swapnode = node->right;
    } else if ( node->right == info->nul )
    {
        swapnode = node->right;
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

bt32_node_t *bt32_find(bt32_info_t *info, int32_t key)
{
    bt32_node_t *current = info->root;
    
    while( current != info->nul )
    {
        if( key == current->key )
        {
            break;
        }
        if( key > current->key )
            current = current->right;
        else
            current = current->left;
    }

    return key == current->key ? current : NULL;
}

void *bt32_get(bt32_info_t *info, int32_t key)
{
    bt32_node_t *current = info->root;
    
    while( current != info->nul )
    {
        if( key == current->key )
        {
            break;
        }
        if( key > current->key )
            current = current->right;
        else
            current = current->left;
    }
    
    return key == current->key ? current->data : NULL;
}

void bt32_traverse(bt32_info_t *bt, void (*cmd)(void *, void *))
{
    bt32_node_t *current, *pre;
    
    current = bt->root;
    while( current != bt->nul )
    {
        if( current->left == bt->nul )
        {
            cmd(current->data, bt->userinfo);
            current = current->right;
        } else
        {
            pre = current->left;
            while(pre->right != bt->nul && pre->right != current)
                pre = pre->right;
            
            if(pre->right == bt->nul)
            {
                pre->right = current;
                current = current->left;
            } else
            {
                pre->right = bt->nul;
                cmd(current->data, bt->userinfo);
                current = current->right;
            } /* End of if condition pre->right == NULL */
        }
    }
}

void bt32_walk(bt32_info_t *info, bt32_node_t *node, void (*cmd)(void *, void *))
{
    if( node != info->nul )
    {
        bt32_walk(info, node->left , cmd);
        cmd(node->data, info->userinfo);
        bt32_walk(info, node->right, cmd);
    }
}

void bt32_turn_right(bt32_info_t *info, bt32_node_t *node)/*{{{*/
{
    bt32_node_t *left = node->left;

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

void bt32_turn_left(bt32_info_t *info, bt32_node_t *node)/*{{{*/
{
    bt32_node_t *right = node->right;

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

void bt32_rb_norm(bt32_info_t *info, bt32_node_t *node)/*{{{*/
{
    bt32_node_t *uncle;
    int left;

    /* node cannot be null at this point. node->parent == NULL means nul node */
    if( !node->parent || !node->parent->parent )
        return; /* no grandparent */

    if( node->parent->color == BT32_BLACK )
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
    
    if( uncle->color == BT32_RED )
    { /* uncle is red */
        uncle->parent->right->color = BT32_BLACK; /* node's uncle */
        uncle->parent->left->color = BT32_BLACK; /* node's parent */
        uncle->parent->color = BT32_RED;
        bt32_rb_norm(info, uncle->parent);
    } else
    {
        if( left && node->parent->right == node)
        {        
            bt32_turn_left(info, node->parent);
            node = node->left;
        } else if( left == 0 && node->parent->left == node )
        {
            bt32_turn_right(info, node->parent);
            node = node->right;
        }
        node->parent->color = BT32_BLACK;
        node->parent->parent->color = BT32_RED;
        if( left )
            bt32_turn_right(info, node->parent->parent);
        else
            bt32_turn_left(info, node->parent->parent);
    }

}/*}}}*/

bt32_node_t *bt32_max(bt32_info_t *info)/*{{{*/
{
    bt32_node_t *current = info->root;

    while( current->right != info->nul )
    {
        current = current->right;
    }
    return current;
}/*}}}*/

bt32_node_t *bt32_min(bt32_info_t *info)/*{{{*/
{
    bt32_node_t *current = info->root;

    while( current->left != info->nul )
    {
        current = current->left;
    }
    return current;
}/*}}}*/
