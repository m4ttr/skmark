#include <stdio.h>
#include <xtables.h>
#include <linux/netfilter/xt_SOMARK.h>

enum {
        O_SOMARK = 0,
};

static void SOMARK_help(void)
{
        printf(
"SOMARK target options:\n"
"  --set-mark int  Set SO_MARK associated socket\n");
}

static void SOMARK_print(const void *ip, const struct xt_entry_target *target,
    int numeric)
{
        const struct xt_somark_tg_info *info = (const void *)target->data;
}

static void SOMARK_save(const void *ip, const struct xt_entry_target *target)
{
        const struct xt_somark_tg_info *info =
            (const struct xt_somark_tg_info *) target->data;
}

static void SOMARK_parse(struct xt_option_call *cb)
{
        struct xt_somark_tg_info *info = cb->data;
        xtables_option_parse(cb);
        switch (cb->entry->id) {
        case O_SOMARK:
            info->somark = cb->val.u32;
            break;
        }
}

static void SOMARK_check(struct xt_fcheck_call *cb)
{
}


static const struct xt_option_entry SOMARK_opts[] = {
        {
            .name   = "set-mark",
            .id     = O_SOMARK,
            .type   = XTTYPE_UINT32,
        },
        XTOPT_TABLEEND,
};

static struct xtables_target somark_tg_reg = {
        .family        = NFPROTO_UNSPEC,
        .name          = "SOMARK",
        .version       = XTABLES_VERSION,
        .size          = XT_ALIGN(sizeof(struct xt_somark_tg_info)),
        .userspacesize = XT_ALIGN(sizeof(struct xt_somark_tg_info)),
        .help          = SOMARK_help,
        .print         = SOMARK_print,
        .save          = SOMARK_save,
        .x6_parse      = SOMARK_parse,
        .x6_fcheck     = SOMARK_check,
        .x6_options    = SOMARK_opts,
};

void _init(void)
{
        xtables_register_target(&somark_tg_reg);
}
