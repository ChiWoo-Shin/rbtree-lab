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
void right_Rotate(rbtree *t, node_t* y){ // 기준이 y node
  node_t* x=y->left; // x 노드가 y의 left child이고 x를 parent로 y를 child로
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
  node_t* y; // Uncle Node
  while(n->parent->color==RBTREE_RED){
    if(n->parent==n->parent->parent->left){ // n의 parent가 할아버지의 왼쪽 child일때
      y=n->parent->parent->right;
      
      if(y->color==RBTREE_RED){ // case 1 삼촌이 red이면
        n->parent->color=RBTREE_BLACK; // 진입노드의 부모를 black으로 칠하고
        y->color=RBTREE_BLACK; // 삼촌 y도 black으로 칠하고
        n->parent->parent->color=RBTREE_RED; // 진입노드의 할아버지를 red로 칠함
        n=n->parent->parent;// 진입노드에 진입노드의 할아버지의 정보를 넣어줌
      }
      else{
        if(n==n->parent->right){ // case2 진입노드가 parent의 오른쪽 child이고 삼촌 y가 black
        n=n->parent;// 진입 노드에 진입노드의 부모의 정보를 넣어주고
        Left_Rotate(t,n); // 좌회전 시킴
      } // case3 진입 노드의 삼촌 y가 black이고 진입노드가 left child인 경우
      n->parent->color=RBTREE_BLACK;  // 진입노드의 parent 를 black으로
      n->parent->parent->color=RBTREE_RED; // 진입 노드의 할아버지를 red로
      right_Rotate(t,n->parent->parent); // 이후 우회전을해서 맞춰줌
      }
    }
    else{ // n의 parent가 할아버지의 오른쪽 자식일때
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
  new_node->parent=y; // 만일 while문을 안타면 root라는 의미니깐 parent가 nil이 되고
  // 그게 아니면 while 문에서 가장 끝 node를 찾아가서 그 값을 넣음
  if(y==t->nil) // 진입 노드가 root라면
    t->root=new_node; // root에 new_node를 넣는다
  else if(key < y->key) // 그게 아니면 이제 key가 y의 key값과 비교해서 어느쪽 child가 될지 정한다
    y->left=new_node;
  else
    y->right=new_node;
  
  new_node->left=t->nil; // 새로운 노드의 left, right child는 nil로 이어짐
  new_node->right=t->nil;
  new_node->color=RBTREE_RED; // 새로운 노드는 red로 칠함
  RB_insert_fixup(t,new_node); // rb tree의 규칙을 지키기위해 fixup 함수 호출
  return new_node; // 모든 내용이 반영된 새로운 node를 return
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
  //u는 최소값의 정보가 v에는 최소값의 child 정보가 들어가있음
  if(u->parent==t->nil) // u의 parent 가 t->nil이면 root라는 의미
    t->root=v; // root에 v를 넣어줌
  else if(u==u->parent->left) // u가 parent의 왼쪽 child 이면
    u->parent->left=v; // u의 parent의 왼쪽 child에 u의 자식을 넣어줌
  else // u가 parent의 right child이면
    u->parent->right=v; // right에 넣어줌
  v->parent=u->parent; // v의 parent에 u의 parent를 넣어줌
}

void rbtree_erase_fixup(rbtree* t,node_t* x){
  while (x!=t->root && x->color==RBTREE_BLACK){
    node_t* temp; // 형제노드 temp
    if(x==x->parent->left){ // 삭제 노드가 왼쪽 child 일때
      temp=x->parent->right; // 형제 노드 설정
      if(temp->color==RBTREE_RED){ // case 1: 형제노드가 red
        temp->color=RBTREE_BLACK; // 형제노드의 색을 black으로 바꾸고
        x->parent->color=RBTREE_RED; // 부모의 색을 red로 변경
        Left_Rotate(t,x->parent); // 그후 좌회전
        temp=x->parent->right; // 형제노드에 삭제노드의 형제노드 정보를 다시 설정
      }
      if(temp->left->color==RBTREE_BLACK && temp->right->color==RBTREE_BLACK){ // case 2: 형제노드가 black, 형제 노드의 두 child가 모두 black
        temp->color=RBTREE_RED; // 형제 노드 색을 red로 바꾸고
        x=x->parent; // 삭제 노드의 parent를 삭제노드로 재설정 (extra black을 해결하기 위해서)
      }
      else{
        if(temp->right->color==RBTREE_BLACK){ // case 3: 형제노드가 black, 형제 노드의 left == red, right == black
          temp->left->color=RBTREE_BLACK; // 형제 노드의 왼쪽 자식을 black으로 칠하고
          temp->color=RBTREE_RED; // 형제 노드를 red로 칠함
          right_Rotate(t,temp); // 오른쪽으로 회전하고
          temp=x->parent->right; // 회전을 통해서 temp는 새로운 형제가 되었기 때문에 다시 형제의 정보를 넣어줌
        }
        temp->color=x->parent->color; // case 4: 형제노드가 black, 형제 노드의  right == red
        x->parent->color=RBTREE_BLACK; // 부모 노드의 색을 black으로 바꿈 (extra black이 타고 올라가서 parent를 염색시킴)
        temp->right->color=RBTREE_BLACK; // 형제의 오른쪽 child도 black으로 염색됨
        Left_Rotate(t,x->parent); // 우회전
        x=t->root; // while 문을 탈출하기 위해서 x에 t-> root를 넣어줌
      }
    }
    else{ // 삭제노드가 오른쪽 child 일때 위와 동일한 방법을 반대로 진행
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
  node_t* y=remove_node; // remove_node 정보를 y에 저장
  color_t y_color=y->color; // remove_node의 색을 y_color에 저장
  node_t* temp; // 삭제 작업을 할때 사용할 temp node 사용
  if (remove_node->left==t->nil){ // remove node의 자식이 1개 (오른쪽에 있음)
    temp=remove_node->right;
    rb_transplant(t,remove_node,remove_node->right);
  }
  else if(remove_node->right==t->nil){ // remove node의 자식이 1개 (왼쪽에 있음)
    temp=remove_node->left;
    rb_transplant(t,remove_node,remove_node->left);
  }
  else{ // remove_node의 자식이 둘
    y=sub_rbtree_min(remove_node->right,t->nil); // y에 삭제될 노드의 오른쪽 subtree에서 최소 값을 저장함
    y_color=y->color; // 최소값의 color를 y_color에 저장함
    temp=y->right; // 최소값의 오른쪽 child를 temp에 저장
    if(y->parent==remove_node) // y의 parent가 삭제될 node이면
      temp->parent=y; // y의 정보를 temp의 parent에 너놓음
    else{ // y의 parent가 삭제될 node가 아니면
      rb_transplant(t,y,y->right); // 최소값과 최소값의 right child를 넣고 최소값의 연결을 다 끊어줌
      y->right=remove_node->right; // 삭제할 node 의 정보를 최소값에 넣어준다
      y->right->parent=y; // right node의 정보가 바뀌었기 때문에 parent node의 정보도 일치시켜줌
    }
    rb_transplant(t,remove_node,y);
    y->left=remove_node->left;
    y->left->parent=y;
    y->color=remove_node->color;
  }

  if (y_color==RBTREE_BLACK)// y가 Black이면 하나 이상의 RB 특성 위반이 발생할수 있음
    rbtree_erase_fixup(t,temp); // 특성 위반을 해결하기 위해 함수를 호출
  
  free(remove_node); // 메모리 누수 방지를 위해서
  remove_node=NULL; // 메모리 누수 방지
  
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
  int index=0;
  int *index_ptr=&index;
  rbtree_to_array_sub(t->root, t->nil, arr, index_ptr); 
  
  return 0;
}

