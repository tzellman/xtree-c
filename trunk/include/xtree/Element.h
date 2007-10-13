#ifndef __XTREE_ELEMENT_H__
#define __XTREE_ELEMENT_H__

#include "xtree/System.h"
#include "xtree/List.h"
#include "xtree/Attribute.h"


/**
 * \struct Element
 * 
 * The Element structure is the basic type for keeping track of the
 * hierarchy of elements/data. A quick example will probably explain
 * it best:
 * <div class="hidden">some div text<span>some span text</span>more text</div>
 * 
 * parent: the <span>'s parent would be the <div> element.
 * tag: <span>'s tag == 'span'
 * attributes: the list of attributes for <div> would contain "class","hidden"
 * children: <span> would be added as one of <div>'s children
 * text: the text "some div text" would be the text for <div>
 * tail: the text "more text" would be the tail for <span>
 */
typedef struct _xtree_Element
{
    struct _xtree_Element *parent;  /** pointer to the parent */
    char *tag;                      /** the tag/identifier */
    xtree_List *attributes;         /** List of Attributes */
    xtree_List *children;           /** List of Element children */
    char *text;                     /** Text contained directly after the start tag */
    char *tail;                     /** Text contained directly after the end tag */
}xtree_Element;


/**
 * Constructs a new Element with the given parent and tag.
 * \param parent    the parent Element
 * \param tag       the tag name
 * \return a new Element*
 */
PUBFUNC(xtree_Element*) xtree_Element_construct(xtree_Element *parent, char *tag);

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
PUBFUNC(xtree_Element*) xtree_Element_addChild(xtree_Element *elem, char *tag);

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
 * Sets the text for this Element. A copy of the passed-in char* is used.
 * The text is the text IMMEDIATELY following the OPENING tag of the Element.
 * For example:
 *      <elem>some text<div></div></elem>
 * "some text" would be the text attribute of the Element.
 * 
 * \param elem  the parent Element to add to
 * \param text  the text of the Element
 */
PUBFUNC(void) xtree_Element_setText(xtree_Element *elem, char *text);

/**
 * Sets the tail for this Element. A copy of the passed-in char* is used.
 * The tail is the text IMMEDIATELY following the ENDING tag of the Element.
 * For example:
 *      <elem>some text<div></div>tail text</elem>
 * "tail text" would be the tail attribute of the <div> Element.
 * 
 * \param elem  the parent Element to add to
 * \param tail  the tail of the Element
 */
PUBFUNC(void) xtree_Element_setTail(xtree_Element *elem, char *tail);

#endif
