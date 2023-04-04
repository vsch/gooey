//
// Created by Vladimir Schneider on 2023-03-26.
//

#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#define EVENT_NONE                  ' '
#define EVENT_MENU_OPTION           '!'
#define EVENT_MENU_NEXT_SELECTION   '>'
#define EVENT_MENU_PREV_SELECTION   '<'
#define EVENT_MENU_INC_SELECTION    '+'
#define EVENT_MENU_DEC_SELECTION    '-'
#define EVENT_MENU_SELECT           '*'
#define EVENT_MESSAGE_CLOSE_MENU    '@'
#define EVENT_MESSAGE_OK            '#'
#define EVENT_MESSAGE_CANCEL        '$'

#define EVENT_CLOSE_MENU            ((char)('0'-1)) // CAUTION: this has to be '0'-1 so can be used as base event, DO NOT CHANGE
#define EVENT_OPTION_0              '0'
#define EVENT_OPTION_1              '1'
#define EVENT_OPTION_2              '2'
#define EVENT_OPTION_3              '3'
#define EVENT_OPTION_4              '4'
#define EVENT_OPTION_5              '5'
#define EVENT_OPTION_6              '6'
#define EVENT_OPTION_7              '7'
#define EVENT_OPTION_8              '8'
#define EVENT_OPTION_9              '9'

typedef char event_t;

#endif //UI_EVENTS_H
