#include<stdio.h>
#include<stdlib.h>

int max(int a, int b) {
    return (a > b)? a : b;
}

// Define la estructura del nodo del AVL tree
struct Node {
    int key;
    int id; // nuevo campo para el ID del nodo
    int height;
    struct Node *left;
    struct Node *right;
    struct Node *child; //nuevo campo para el nodo hijo
};

// Obtiene la altura de un nodo
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Crea un nuevo nodo
struct Node* newNode(int key, int id) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->id = id;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    node->child = NULL;
    return(node);
}


// Inserta un nuevo valor en el AVL tree
struct Node* insert(struct Node* node, int key, int id) {
    
    // Realiza una inserción normal en un árbol binario de búsqueda
    if (node == NULL) {
        // mainRoot = ;
        // puts("insertado nodo raiz");
        return(newNode(key, id));
    }

    if (key < node->key) {
        node->left = insert(node->left, key, id);
        node->left->child = node; // se asigna el nodo actual como el padre del nodo insertado
    } else if (key > node->key) {
        node->right = insert(node->right, key, id);
        node->right->child = node; // se asigna el nodo actual como el padre del nodo insertado
    } else {// No se permite tener valores repetidos
        puts("valor repetido al insertar");
        return node;
    }

    // Actualiza la altura del nodo actual
    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    return node;

}

void freeAVLTree(struct Node* node){
    if(node==NULL)
        return;
    freeAVLTree(node->left);
    freeAVLTree(node->right);
    free(node);
}

struct Node *minValueNode(struct Node* node) {
    struct Node* current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}


// Elimina un nodo en particular
struct Node* deleteNode(struct Node* root, int key) {
    // Realiza una eliminación normal en un árbol binario de búsqueda
    if (root == NULL)
        return root;

    // Si la clave a eliminar es menor que la clave del nodo raíz, entonces está en el subárbol izquierdo
    if ( key < root->key )
        root->left = deleteNode(root->left, key);

    // Si la clave a eliminar es mayor que la clave del nodo raíz, entonces está en el subárbol derecho
    else if( key > root->key )
        root->right = deleteNode(root->right, key);

    // si la clave es igual a la clave del nodo raíz, entonces esta es la clave a eliminar
    else {
        // nodo con solo un hijo o sin hijos
        if( (root->left == NULL) || (root->right == NULL) ) {
            struct Node *temp = root->left ? root->left : root->right;

            // Sin hijos
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else // Un hijo
            *root = *temp; // Copia el contenido del hijo

            free(temp);
        }
        else {
            // Nodo con dos hijos: Obtiene el predecesor inorder (el mayor en el subarbol izquierdo)
            struct Node* temp = minValueNode(root->right);

            // Copia el valor del predecesor inorder en el nodo actual
            root->key = temp->key;

            // Elimina el predecesor inorder
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // Si el árbol sólo tiene un nodo, entonces retorna ese nodo
    if (root == NULL)
      return root;

    // Actualiza la altura del nodo actual
    root->height = 1 + max(height(root->left), height(root->right));

    return root;
}

void preOrder(struct Node *root) {
    if(root != NULL) {
        printf("%d (id: %d) ", root->key, root->id);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(struct Node *root) {
    if(root != NULL) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

void postOrder(struct Node *root) {
    if(root != NULL) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d (id: %d) ", root->key, root->id);
    }
}


// struct Node* search(int id) {
//     return searchHelper(root, id);
// }

struct Node* searchHelper(struct Node* node, int id) {
    // Si el nodo es nulo o el id coincide, retorna el nodo
    if (node == NULL || node->id == id)
        return node;

    // Busca en el subárbol izquierdo
    struct Node* left = searchHelper(node->left, id);
    if(left != NULL)
        return left;

    // Busca en el subárbol derecho
    return searchHelper(node->right, id);
}