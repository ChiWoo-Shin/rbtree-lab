#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color=RBTREE_BLACK;
  nil->key=0;
  nil->left=nil->right=nil->parent=NULL;  

  p->nil=nil;
  p->root=p->nil;

  return p;
}

// node_t* make_new_node(rbtree* t, const key_t key){
//   node_t* New_node=(node_t*)malloc(sizeof(node_t));
//   New_node->color=RBTREE_RED;
//   New_node->key=key;
//   New_node->left=t->nil;
//   New_node->right=t->nil;
//   New_node->parent=t->nil;

//   return New_node;
// }

// 좌 회전
void Left_Rotate(rbtree *t, node_t* x){
  node_t* y=x->right; // y는 x의 오른쪽 child
  x->right=y->left; // x의 오른쪽 child = y의 left child로 변경
  if (y->left!= t->nil) // y 의 left child가 NULL이 아닐때
    y->left->parent=x; // y의 left child의 parent를 x로 옮김
  y->parent=x->parent; // y의 parent 에는 x의 parent를 넣음
  if(x->parent==t->nil) // x의 parent가 NULL 이면
    t->root=y; // x가 root라는거니깐 t에 y를 바로 넣음
  else if(x==x->parent->left) // x가 x의 parent의 left child이면
    x->parent->left=y; // 거기에 y를 넣음
  else // x가 parent의 right child 이면
    x->parent->right=y; // 거기에 y를 넣음
  y->left=x; // y의 left child에 x를 넣음
  x->parent=y; // x의 parent는 y가 됨 
} // 이 순서를 통해서 x가 부모이고 y가 right child 였던게 y가 부모가 되고 x가 left child 로 변경됨

//우 회전
void right_Rotate(rbtree *t, node_t* y){
  node_t* x=y->left;
  y->left=x->right;
  if (x->right!=t->nil)
    x->right->parent=y;
  x->parent=y->parent;
  if(y->parent==t->nil)
    t->root=x;
  else if(y==y->parent->left)
    y->parent->left=x;
  else
    y->parent->right=x;
  x->right=y;
  y->parent=x;
}

void delete_sub(node_t *t, node_t* nil){
  if(t!=nil){
    delete_sub(t->left,nil);
    delete_sub(t->right,nil);
    free(t);
  }  
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_sub(t->root,t->nil);
  free(t->nil);  
  free(t);
  t=NULL;
}

void RB_insert_fixup(rbtree *t, node_t* n){
  node_t* y;
  while(n->parent->color==RBTREE_RED){
    if(n->parent==n->parent->parent->left){
      y=n->parent->parent->right;
      
      if(y->color==RBTREE_RED){
        n->parent->color=RBTREE_BLACK;
        y->color=RBTREE_BLACK;
        n->parent->parent->color=RBTREE_RED;
        n=n->parent->parent;      
      }
      else{
        if(n==n->parent->right){
        n=n->parent;
        Left_Rotate(t,n);
      }
      n->parent->color=RBTREE_BLACK;
      n->parent->parent->color=RBTREE_RED;
      right_Rotate(t,n->parent->parent);
      }
    }
    else{
      y=n->parent->parent->left;
      if(y->color==RBTREE_RED){
        n->parent->color=RBTREE_BLACK;
        y->color=RBTREE_BLACK;
        n->parent->parent->color=RBTREE_RED;
        n=n->parent->parent;      
      }
      else{
       if(n==n->parent->left){
        n=n->parent;
        right_Rotate(t,n);
      }      
      n->parent->color=RBTREE_BLACK;
      n->parent->parent->color=RBTREE_RED;
      Left_Rotate(t,n->parent->parent);
      }
    }
  }
  t->root->color=RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* new_node=(node_t*)calloc(1, sizeof(node_t));
  new_node->key=key;
  node_t* y=t->nil;
  node_t* x=t->root;
  
  while(x!=t->nil){ // 키 값의 위치를 찾는다
    y=x; // y의 빈 node에 x 값을 너놓고 --> 추후에 사용한다
    if(key<x->key) // inser하려는 key값이 x의 key보다 작으면 왼쪽 크면 오른쪽
      x=x->left;
    else
      x=x->right;
  }
  new_node->parent=y;
  if(y==t->nil)
    t->root=new_node;
  else if(key < y->key)
    y->left=new_node;
  else
    y->right=new_node;
  
  new_node->left=t->nil;
  new_node->right=t->nil;
  new_node->color=RBTREE_RED;
  RB_insert_fixup(t,new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
    node_t *p=t->root;
    while(p!=t->nil){
        if(p->key==key){
            return p;
        }
        else if(p->key<key){
            p=p->right;
        }
        else{
            p=p->left;
        }
    }
    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* min_node=t->root;
  while(min_node->left !=t->nil){
    min_node=min_node->left;    
  }    
  return min_node;
}

node_t *sub_rbtree_min(node_t* t,node_t* nil) {
  // TODO: implement find
  node_t* min_node=t;
  while(min_node->left !=nil){
    min_node=min_node->left;    
  }    
  return min_node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* max_node=t->root;
  while(max_node->right !=t->nil){
    max_node=max_node->right;
  }    
  return max_node;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){ // erase가 호출하는 서브 루틴
  if(u->parent==t->nil)
    t->root=v;
  else if(u==u->parent->left)
    u->parent->left=v;
  else
    u->parent->right=v;
  v->parent=u->parent;
}

void rbtree_erase_fixup(rbtree* t,node_t* x){
  while (x!=t->root && x->color==RBTREE_BLACK){
    node_t* temp;
    if(x==x->parent->left){
      temp=x->parent->right;
      if(temp->color==RBTREE_RED){
        temp->color=RBTREE_BLACK;
        x->parent->color=RBTREE_RED;
        Left_Rotate(t,x->parent);
        temp=x->parent->right;
      }
      if(temp->left->color==RBTREE_BLACK && temp->right->color==RBTREE_BLACK){
        temp->color=RBTREE_RED;
        x=x->parent;
      }
      else{
        if(temp->right->color==RBTREE_BLACK){
          temp->left->color=RBTREE_BLACK;
          temp->color=RBTREE_RED;
          right_Rotate(t,temp);
          temp=x->parent->right;
        }
        temp->color=x->parent->color;
        x->parent->color=RBTREE_BLACK;
        temp->right->color=RBTREE_BLACK;
        Left_Rotate(t,x->parent);
        x=t->root;
      }
    }
    else{
      temp=x->parent->left;
      if(temp->color==RBTREE_RED){
        temp->color=RBTREE_BLACK;
        x->parent->color=RBTREE_RED;
        right_Rotate(t,x->parent);
        temp=x->parent->left;
      }
      if(temp->right->color==RBTREE_BLACK && temp->left->color==RBTREE_BLACK){
        temp->color=RBTREE_RED;
        x=x->parent;
      }
      else{
        if(temp->left->color==RBTREE_BLACK){
          temp->right->color=RBTREE_BLACK;
          temp->color=RBTREE_RED;
          Left_Rotate(t,temp);
          temp=x->parent->left;
        }
        temp->color=x->parent->color;
        x->parent->color=RBTREE_BLACK;
        temp->left->color=RBTREE_BLACK;
        right_Rotate(t,x->parent);
        x=t->root;
      }
    }
  }
  x->color=RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *remove_node) {
  // TODO: implement erase
  node_t* y=remove_node;
  color_t y_color=y->color;
  node_t* temp;
  if (remove_node->left==t->nil){
    temp=remove_node->right;
    rb_transplant(t,remove_node,remove_node->right);
  }
  else if(remove_node->right==t->nil){
    temp=remove_node->left;
    rb_transplant(t,remove_node,remove_node->left);
  }
  else{
    y=sub_rbtree_min(remove_node->right,t->nil);
    y_color=y->color;
    temp=y->right;
    if(y->parent==remove_node)
      temp->parent=y;
    else{
      rb_transplant(t,y,y->right);
      y->right=remove_node->right;
      y->right->parent=y;
    }
    rb_transplant(t,remove_node,y);
    y->left=remove_node->left;
    y->left->parent=y;
    y->color=remove_node->color;
  }

  if (y_color==RBTREE_BLACK)
    rbtree_erase_fixup(t,temp);
  
  free(remove_node);
  remove_node=NULL;

  
  return 0;
}


void rbtree_to_array_sub(node_t* k,node_t *nil, key_t *arr, int *index_ptr){  
  if(k==nil) return;
  rbtree_to_array_sub(k->left,nil,arr,index_ptr);
  arr[*(index_ptr)]= k->key;
  *index_ptr+=1;
  
  rbtree_to_array_sub(k->right,nil,arr,index_ptr);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int *array_index = calloc(1,sizeof(int));
  int index=0;
  int *index_ptr=&index;
  rbtree_to_array_sub(t->root, t->nil, arr, index_ptr); 
  free(array_index);
  return 0;
}
