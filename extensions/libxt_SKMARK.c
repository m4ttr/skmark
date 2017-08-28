/* Shared library add-on to iptables for SKMARK
 *
 * libxt_SKMARK.c borrowed heavily from xt_socket.c
 *
 * TODO: add support for ! skmark
 *
 */
#include <stdio.h>
#include <xtables.h>
#include <netfilter/xt_SKMARK.h>

enum {
    O_SKMARK = 0,
};

static const struct xt_option_entry SKMARK_opts[] = {
    {.name   = "set-mark", .id = O_SKMARK, .type   = XTTYPE_UINT32},
    XTOPT_TABLEEND,
};

static void SKMARK_help(void)
{
        printf(
"SKMARK target options:\n"
"  --set-mark int  Set SO_MARK associated socket (ex: 3)\n");
}

static void SKMARK_print(const void *ip, const struct xt_entry_target *target,
    int numeric)
{
    const struct xt_skmark_tg_info *info = (const void *)target->data;
    printf(" SKMARK");
    printf(" set %i", info->skmark);
}

static void SKMARK_save(const void *ip, const struct xt_entry_target *target)
{
    const struct xt_skmark_tg_info *info =
        (const struct xt_skmark_tg_info *) target->data;
    printf(" SKMARK Set %i", info->skmark);
}

static void SKMARK_parse(struct xt_option_call *cb)
{
    struct xt_skmark_tg_info *info = cb->data;
    xtables_option_parse(cb);
    switch (cb->entry->id) {
    case O_SKMARK:
        info->skmark = cb->val.u32;
        break;
    }
}

static struct xtables_target skmark_tg_reg = {
    .family        = NFPROTO_UNSPEC,
    .name          = "SKMARK",
    .version       = XTABLES_VERSION,
    .size          = XT_ALIGN(sizeof(struct xt_skmark_tg_info)),
    .userspacesize = XT_ALIGN(sizeof(struct xt_skmark_tg_info)),
    .help          = SKMARK_help,
    .print         = SKMARK_print,
    .save          = SKMARK_save,
    .x6_parse      = SKMARK_parse,
    .x6_options    = SKMARK_opts,
};

void _init(void)
{
    xtables_register_target(&skmark_tg_reg);
}
