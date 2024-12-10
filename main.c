#include <stdio.h>
#include <stdlib.h>

typedef enum { false, true } boolean;

typedef struct node {
    int info;
    struct node* left, * right;
    boolean lthread, rthread;
} node;

node* insert(node* root, int ikey) {
    node* tmp, * par, * ptr;
    int found = 0;
    ptr = root;
    par = NULL;
    while (ptr != NULL) {
        if (ikey == ptr->info) {
            found = 1;
            break;
        }
        par = ptr;
        if (ikey < ptr->info) {
            if (ptr->lthread == false)
                ptr = ptr->left;
            else
                break;
        }
        else {
            if (ptr->rthread == false)
                ptr = ptr->right;
            else
                break;
        }
    }
    if (found) {
        printf("\nDuplicate key");
    }
    else {
        tmp = (node*)malloc(sizeof(node));
        tmp->info = ikey;
        tmp->lthread = true;
        tmp->rthread = true;

        if (par == NULL) {
            root = tmp;
            tmp->left = NULL;
            tmp->right = NULL;
        }
        else if (ikey < par->info) {
            tmp->left = par->left;
            tmp->right = par;
            par->lthread = false;
            par->left = tmp;
        }
        else {
            tmp->left = par;
            tmp->right = par->right;
            par->rthread = false;
            par->right = tmp;
        }
    }
    return root;
}

node* in_succ(node* ptr) {
    if (ptr->rthread == true) {
        return ptr->right;
    }
    else {
        ptr = ptr->right;
        while (ptr->lthread == false) {
            ptr = ptr->left;
        }
        return ptr;
    }
}

node* in_pred(node* ptr) {
    if (ptr->lthread == true) {
        return ptr->left;
    }
    else {
        ptr = ptr->left;
        while (ptr->rthread == false) {
            ptr = ptr->right;
        }
        return ptr;
    }
}

node* case_a(node* root, node* par, node* ptr) {
    if (par == NULL) {
        root = NULL;
    }
    else if (ptr == par->left) {
        par->lthread = true;
        par->left = ptr->left;
    }
    else {
        par->rthread = true;
        par->right = ptr->right;
    }
    free(ptr);
    return root;
}

node* case_b(node* root, node* par, node* ptr) {
    node* child, * s, * p;
    if (ptr->lthread == false) {
        child = ptr->left;
    }
    else {
        child = ptr->right;
    }
    if (par == NULL) {
        root = child;
    }
    else if (ptr == par->left) {
        par->left = child;
    }
    else {
        par->right = child;
    }
    s = in_succ(ptr);
    p = in_pred(ptr);
    if (ptr->lthread == false) {
        p->right = s;
    }
    else if (ptr->rthread == false) {
        s->left = p;
    }
    free(ptr);
    return root;
}

node* case_c(node* root, node* par, node* ptr) {
    node* succ, * parsucc;
    parsucc = ptr;
    succ = ptr->right;
    while (succ->left != NULL) {
        parsucc = succ;
        succ = succ->left;
    }
    ptr->info = succ->info;
    if (succ->lthread == true && succ->rthread == true) {
        root = case_a(root, parsucc, succ);
    }
    else {
        root = case_b(root, parsucc, succ);
    }
    return root;
}

node* del(node* root, int dkey) {
    node* par, * ptr;
    int found = 0;
    ptr = root;
    par = NULL;
    while (ptr != NULL) {
        if (dkey == ptr->info) {
            found = 1;
            break;
        }
        par = ptr;
        if (dkey < ptr->info) {
            if (ptr->lthread == false) {
                ptr = ptr->left;
            }
            else {
                break;
            }
        }
        else {
            if (ptr->rthread == false) {
                ptr = ptr->right;
            }
            else {
                break;
            }
        }
    }
    if (found == 0) {
        printf("\ndkey not present in tree");
    }
    else if (ptr->lthread == false && ptr->rthread == false) {
        root = case_c(root, par, ptr);
    }
    else if (ptr->lthread == false) {
        root = case_b(root, par, ptr);
    }
    else if (ptr->rthread == false) {
        root = case_b(root, par, ptr);
    }
    else {
        root = case_a(root, par, ptr);
    }
    return root;
}

void inorder(node* root) {
    node* ptr;
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }
    ptr = root;
    while (ptr->lthread == false) {
        ptr = ptr->left;
    }
    while (ptr != NULL) {
        printf("%d ", ptr->info);
        ptr = in_succ(ptr);
    }
}

void preorder(node* root) {
    node* ptr;
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }
    ptr = root;
    while (ptr != NULL) {
        printf("%d ", ptr->info);
        if (ptr->lthread == false) {
            ptr = ptr->left;
        }
        else if (ptr->rthread == false) {
            ptr = ptr->right;
        }
        else {
            while (ptr != NULL && ptr->rthread == true) {
                ptr = ptr->right;
            }
            if (ptr != NULL) {
                ptr = ptr->right;
            }
        }
    }
}

int main() {
    int choice, num;
    node* root = NULL;
    do {
        printf("\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Inorder Traversal\n");
        printf("4. Preorder Traversal\n");
        printf("5. Quit\n");
        printf("\nEnter your choice : ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                printf("\nEnter the number to be inserted : ");
                scanf("%d", &num);
                root = insert(root, num);
                break;
            }
            case 2: {
                printf("\nEnter the number to be deleted : ");
                scanf("%d", &num);
                root = del(root, num);
                break;
            }
            case 3: {
                inorder(root);
                break;
            }
            case 4: {
                preorder(root);
                break;
            }
            case 5: {
                break;
            }
            default: {
                printf("\nWrong choice\n");
            }
        }
    } while (choice != 5);
    return 0;
}