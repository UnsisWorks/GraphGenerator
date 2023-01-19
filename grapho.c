#include<stdio.h>
#include<stdlib.h>

int max(int a, int b) {
    return (a > b)? a : b;
}


// Define la estructura del nodo del AVL tree
struct Node {
    int key;
    int height;
    struct Node *left;
    struct Node *right;
};

// Obtiene la altura de un nodo
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Obtiene el factor de equilibrio de un nodo
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Crea un nuevo nodo
struct Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return(node);
}

// Realiza una rotación simple a la derecha
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    // Realiza la rotación
    x->right = y;
    y->left = T2;

    // Actualiza las alturas
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Retorna el nodo raíz actual
    return x;
}

// Realiza una rotación simple a la izquierda
struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    // Realiza la rotación
    y->left = x;
    x->right = T2;

    // Actualiza las alturas
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Retorna el nodo raíz actual
    return y;
}

// Inserta un nuevo valor en el AVL tree
struct Node* insert(struct Node* node, int key) {
    // Realiza una inserción normal en un árbol binario de búsqueda
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left  = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // No se permite tener valores repetidos
        return node;

    // Actualiza la altura del nodo actual
    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    // Obtiene el factor de equilibrio del nodo actual
  int balance = getBalance(node);
  // Si el factor de equilibrio es mayor a 1 o menor a -1, entonces el árbol se ha desequilibrado
  // y se necesita realizar una rotación

  // Caso Left Left
  if (balance > 1 && key < node->left->key)
    return rightRotate(node);

  // Caso Right Right
  if (balance < -1 && key > node->right->key)
    return leftRotate(node);

  // Caso Left Right
  if (balance > 1 && key > node->left->key) {
    node->left =  leftRotate(node->left);
    return rightRotate(node);
  }

  // Caso Right Left
  if (balance < -1 && key < node->right->key) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  // Si no se desequilibra, retorna el nodo raíz actual
  return node;

}

void preOrder(struct Node *root) {
    if(root != NULL) {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
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

    // Obtiene el factor de equilibrio del nodo actual
    int balance = getBalance(root);

    // Si el factor de equilibrio es mayor a 1 o menor a -1, entonces el árbol se ha desequilibrado y se necesita realizar una rotación

    // Caso Left Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Caso Left Right
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Caso Right Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Caso Right Left
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
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
        printf("%d ", root->key);
    }
}
