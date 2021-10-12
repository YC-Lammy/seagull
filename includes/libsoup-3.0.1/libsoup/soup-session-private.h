/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2000-2003, Ximian, Inc.
 */

#ifndef __SOUP_SESSION_PRIVATE_H__
#define __SOUP_SESSION_PRIVATE_H__ 1

#include "soup-session.h"
#include "soup-content-processor.h"

G_BEGIN_DECLS

void     soup_session_requeue_message       (SoupSession *session,
					     SoupMessage *msg);
void     soup_session_pause_message         (SoupSession *session,
					     SoupMessage *msg);
void     soup_session_unpause_message       (SoupSession *session,
					     SoupMessage *msg);
void     soup_session_cancel_message        (SoupSession *session,
					     SoupMessage *msg);

SoupMessage *soup_session_get_original_message_for_authentication (SoupSession *session,
								   SoupMessage *msg);

GInputStream *soup_session_setup_message_body_input_stream (SoupSession        *session,
                                                            SoupMessage        *msg,
                                                            GInputStream       *body_stream,
                                                            SoupProcessingStage start_at_stage);

GSList       *soup_session_get_features                    (SoupSession        *session,
							    GType               feature_type);

G_END_DECLS

#endif /* __SOUP_SESSION_PRIVATE_H__ */
