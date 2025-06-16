//Edgardo Adrián Franco Martínez - Mayo 2021

#include "TADArbol_bin.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void Initialize(arbol_binario *a)
{
	*a=NULL;
	return;
}

void Destroy(arbol_binario *a)
{
	if(*a!=NULL)
	{
		if((*a)->izq!=NULL)
			Destroy(&((*a)->izq));
		if((*a)->der!=NULL) //(*(*a))).der
			Destroy(&((*a)->der));
		free(*a);
		*a=NULL;
	}
	return;
}

void NewRightSon(arbol_binario *a,posicion p,elemento e)
{
	if(Empty(&(*a)))
	{	
		*a=malloc(sizeof(nodo));
		if(*a==NULL)
		{
			printf("ERROR: NewRightSon(a,p,e) desbordamiento del arbol");
			exit(1);
		}
		(*a)->e=e;
		(*a)->der=NULL;
		(*a)->izq=NULL;		
	}
	else
	{
		if(!NullNode(&(*a),p))
		{
			if(p->der!=NULL)
			{
				printf("ERROR: NewRightSon(a,p,e) p ya tiene un hijo derecho");
				exit(1);
			}
			p->der=malloc(sizeof (nodo));
			p->der->e=e;
			p->der->der=NULL;
			p->der->izq=NULL;
		}
		else
		{
			printf("ERROR (NewRightSon):La posición dada no es valida");
			exit(1);
		}
	}
	return;
}

void NewLeftSon(arbol_binario *a,posicion p,elemento e)
{
	if(Empty(&(*a)))
	{	
		*a=malloc(sizeof(nodo));
		(*a)->e=e;
		(*a)->der=NULL;
		(*a)->izq=NULL;		
	}
	else
	{
		if(!NullNode(&(*a),p))
		{
			if(p->izq!=NULL)
			{
				printf("ERROR: NewLeftSon(a,p,e) p ya tiene un hijo izquierdo");
				exit(1);
			}
			p->izq=malloc(sizeof (nodo));
			p->izq->e=e;
			p->izq->der=NULL;
			p->izq->izq=NULL;
		}
		else
		{
			printf("ERROR (NewLeftSon):La posición dada no es valida");
			exit(1);
		}
	}
	
	return;
}

void DeleteRightSon(arbol_binario *a,posicion p)
{
	if(!NullNode(&(*a),p))
	{
		Destroy(&(p->der));
		p->der = NULL;
	}
	else
	{
		printf("ERROR (DeleteRightSon):La posición dada no es valida");
		exit(1);
	}
	return;
}

void DeleteLeftSon(arbol_binario *a,posicion p)
{
	if(!NullNode(&(*a),p))
	{
		Destroy(&(p->izq));
		p->izq = NULL;
	}
	else
	{
		printf("ERROR (DeleteLeftSon):La posición dada no es valida");
		exit(1);
	}		
	return;
}

void DeleteNode(arbol_binario *a,posicion p)
{
	if(!NullNode(&(*a),p))
	{
		posicion padre=Parent(a,p);
		
		if (padre->der==p)
			padre->der=NULL;
		else 
			if (padre->izq==p)
				padre->izq=NULL;
		Destroy(&p);	
	}
	else
	{
		printf("ERROR (DeleteNode):La posición dada no es valida");
		exit(1);
	}	
	return;
}

void ReplaceNode(arbol_binario *a,posicion p,elemento e)
{
	if(!NullNode(&(*a),p))
	{
		p->e=e;
	}
	else
	{
		printf("ERROR (ReplaceNode):La posición dada no es valida");
		exit(1);
	}		
		return;
}

posicion Root(arbol_binario *a)
{
	return *a;
}

posicion Parent(arbol_binario *a,posicion p)
{
	posicion padre=NULL;
	if(*a!=NULL)
	{	
		if((*a)->der==p || (*a)->izq==p)
			return *a;
		if((*a)->izq!=NULL)
			padre=Parent(&((*a)->izq),p);
		if((*a)->der!=NULL && padre==NULL)
			padre=Parent(&((*a)->der),p);
	}
	else
	{
		printf("ERROR (Parent):La posición dada no es valida");
		exit(1);
	}		
	return padre;	
}

posicion RightSon(arbol_binario *a,posicion p)
{
	if(!NullNode(&(*a),p))
	{
		return p->der;
	}
	else
	{
		printf("ERROR (RightSon):La posición dada no es valida");
		exit(1);
	}	
}
posicion LeftSon(arbol_binario *a,posicion p)
{
	if(!NullNode(&(*a),p))
	{
		return p->izq;
	}
	else
	{
		printf("ERROR (LeftSon):La posición dada no es valida");
		exit(1);
	}			
}

posicion Search(arbol_binario *a,elemento e)
{
	posicion p=NULL;
	if(Empty(&(*a)))
		return p;
	if (memcmp(&((*a)->e),&e,sizeof(elemento))==0)
		return *a;
	if((*a)->izq!=NULL)
		p=Search(&((*a)->izq),e);
	if((*a)->der!=NULL && p==NULL)
		p=Search(&((*a)->der),e);

	return p;
}

boolean Empty(arbol_binario *a)
{
	if(*a!=NULL)
		return FALSE;
	else
		return TRUE;	
}

boolean NullNode(arbol_binario *a,posicion p)
{
	boolean b = TRUE; 
	if(*a==NULL||p==NULL)
		return TRUE;
	if (*a==p)
		return FALSE;
	if((*a)->izq!=NULL)
		b=NullNode(&((*a)->izq),p);
	if((*a)->der!=NULL && b==TRUE)
		b=NullNode(&((*a)->der),p);

	return b;
}

elemento ReadNode(arbol_binario *a,posicion p)
{
	if(!NullNode(&(*a),p))
	{
		return p->e;
	}
	else
	{
		printf("ERROR (ReadNode):La posición dada no es valida");
		exit(1);
	}	
}
