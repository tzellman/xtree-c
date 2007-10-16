#ifndef __XTREE_ELEMENT_H__
#define __XTREE_ELEMENT_H__

#include "xtree/System.h"
#include "xtree/List.h"
#include "xtree/Attribute.h"


/**
 * Enumeration of Element types
 */
enum
{
    XTREE_ELEMENT = 0,      /** The basic element type */
    XTREE_TEXT,             /** An element that is a text node */
    XTREE_COMMENT,          /** An element representing a comment */
    XTREE_CDATA,            /** An element representing CDATA */
    XTREE_ROOT              /** Only used for top-level root elements */
};

/**
 * \struct Element
 * 
 * The Element structure is the basic type for keeping track of the
 * hierarchy of elements/data. A quick example will probably explain
 * it best:
 * <div class="hidden">some div text<!-- a comment -->
 *      <span>some span text</span>more text</div>
 * 
 * - the <div> tag is the top-most parent Element
 * - the <div> tag's only attribute has (name,value) of ('class', '"hidden"')
 * - "some div text" would be the first child Element of <div>, of type "TEXT";
 *   its data attribute would be "some div text"
 * - <!-- a comment --> is the 2nd child of <div>, of type "COMMENT";
 *   its data attribute would be "<!-- a comment -->"
 * - <span> is a child of <div>
 * - "some span text" is a child of <span>, of type "TEXT"
 * - ...
 */
typedef struct _xtree_Element
{
    struct _xtree_Element *parent;  /** Pointer to the parent */
    int elemType;                   /** The type of the Element (ELEMENT, TEXT, COMMENT) */
    char *tag;                      /** the tag/identifier */
    xtree_List *attributes;         /** List of Attributes */
    xtree_List *children;           /** List of Element children */
    char *data;                     /** (optional) data (used for text and comment nodes) */
    int isEmpty;                    /** true if the element should be represented as an empty element */
}xtree_Element;


/**
 * Constructs a new Element with the given parent and tag.
 * \param parent    the parent Element
 * \param tag       the tag name
 * \return a new Element*
 */
PUBFUNC(xtree_Element*) xtree_Element_construct(xtree_Element *parent,
                                                char *tag,
                                                int elemType);

/**
 * Destroys the Element
 */
PUBFUNC(void) xtree_Element_destruct(xtree_Element **elem);

/**
 * Creates and adds a child element to this element
 * \param elem  the parent Element to add to
 * \param tag   the tag of the new Element.
 * \return a new Element*
 */
PUBFUNC(xtree_Element*) xtree_Element_addChild(xtree_Element *elem,
                                               char *tag,
                                               int elemType);

/**
 * Shortcut for:
 *      Element *newElem = xtree_Element_addChild(elem, NULL, XTREE_TEXT);
 *      xtree_Element_setData(newElem, data);
 */ 
PUBFUNC(xtree_Element*) xtree_Element_addTextChild(xtree_Element *elem,
                                                   char *data);

/**
 * Shortcut for:
 *      Element *newElem = xtree_Element_addChild(elem, NULL, XTREE_CDATA);
 *      xtree_Element_setData(newElem, data);
 */ 
PUBFUNC(xtree_Element*) xtree_Element_addCDataChild(xtree_Element *elem,
                                                    char *data);

/**
 * Shortcut for:
 *      Element *newElem = xtree_Element_addChild(elem, NULL, XTREE_COMMENT);
 *      xtree_Element_setData(newElem, data);
 */ 
PUBFUNC(xtree_Element*) xtree_Element_addCommentChild(xtree_Element *elem,
                                                      char *data);

/**
 * Creates and adds an Attribute to this element
 * \param elem  the parent Element to add to
 * \param name  the name of the attribute
 * \param value the value of the attribute
 * \return a new Attribute*
 */
PUBFUNC(xtree_Attribute*) xtree_Element_addAttribute(xtree_Element *elem,
                                                     char *name,
                                                     char *value);

/**
 * Sets the data for this Element. A copy of the passed-in char* is used.
 * The data attribute is only used for certain Element types (TEXT, COMMENT).
 * For example:
 *      <elem>some text<div></div></elem>
 * "some text" would be the first child of <elem>, of type "TEXT"
 * 
 * \param elem  the parent Element to add to
 * \param data  the data of the Element
 */
PUBFUNC(void) xtree_Element_setData(xtree_Element *elem, char *data);

#endif
