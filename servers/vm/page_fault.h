/** @file page_fault.h */
#pragma once
#include <libs/common/types.h>

struct task;

error_t handle_page_fault(struct task *task, uaddr_t vaddr, uaddr_t ip,
                          unsigned fault);
