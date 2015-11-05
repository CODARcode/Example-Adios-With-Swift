/*
  File autogenerated by gengetopt version 2.22.4
  generated with the following command:
  gengetopt --input=icee_cmdline.ggo --func-name=icee_cmdline_parser --file=icee_cmdline --arg-struct-name=icee_args_info

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FIX_UNUSED
#define FIX_UNUSED(X) (void) (X) /* avoid warnings for unused params */
#endif

#include <getopt.h>

#include "icee_cmdline.h"

const char *icee_args_info_purpose = "";

const char *icee_args_info_usage = "Usage: icee [OPTIONS]...";

const char *icee_args_info_description = "";

const char *icee_args_info_help[] = {
  "  -h, --help                Print help and exit",
  "  -V, --version             Print version and exit",
  "  -c, --client              Client mode  (default=off)",
  "  -w, --writemethod=STRING  ADIOS write method  (default=`POSIX1')",
  "  -r, --readmethod=STRING   ADIOS read method  (default=`BP')",
  "  -n, --len=INT             array length",
  "      --timeout=FLOAT       Timeout",
  "      --host=STRING         local hostname  (default=`localhost')",
  "  -p, --port=INT            local port  (default=`59997')",
  "  -s, --remotehost=STRING   remote hostname  (default=`localhost')",
  "  -t, --remoteport=INT      remote port  (default=`59999')",
  "  -T, --method=STRING       EVPath method  (default=`TCP')",
  "  -v, --verbose=INT         verbosity level  (default=`3')",
  "  -e, --contact=STRING      EVPath contact string",
  "      --passive             enable passive mode  (default=off)",
  "      --nclient=INT         maximum number of clients",
  "      --isnative            use native contact  (default=off)",
  "      --remotelist=STRING   remote list",
  "      --attrlist=STRING     attr list",
    0
};

typedef enum {ARG_NO
  , ARG_FLAG
  , ARG_STRING
  , ARG_INT
  , ARG_FLOAT
} icee_cmdline_parser_arg_type;

static
void clear_given (struct icee_args_info *args_info);
static
void clear_args (struct icee_args_info *args_info);

static int
icee_cmdline_parser_internal (int argc, char **argv, struct icee_args_info *args_info,
                        struct icee_cmdline_parser_params *params, const char *additional_error);


static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct icee_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->client_given = 0 ;
  args_info->writemethod_given = 0 ;
  args_info->readmethod_given = 0 ;
  args_info->len_given = 0 ;
  args_info->timeout_given = 0 ;
  args_info->host_given = 0 ;
  args_info->port_given = 0 ;
  args_info->remotehost_given = 0 ;
  args_info->remoteport_given = 0 ;
  args_info->method_given = 0 ;
  args_info->verbose_given = 0 ;
  args_info->contact_given = 0 ;
  args_info->passive_given = 0 ;
  args_info->nclient_given = 0 ;
  args_info->isnative_given = 0 ;
  args_info->remotelist_given = 0 ;
  args_info->attrlist_given = 0 ;
}

static
void clear_args (struct icee_args_info *args_info)
{
  FIX_UNUSED (args_info);
  args_info->client_flag = 0;
  args_info->writemethod_arg = gengetopt_strdup ("POSIX1");
  args_info->writemethod_orig = NULL;
  args_info->readmethod_arg = gengetopt_strdup ("BP");
  args_info->readmethod_orig = NULL;
  args_info->len_orig = NULL;
  args_info->timeout_orig = NULL;
  args_info->host_arg = gengetopt_strdup ("localhost");
  args_info->host_orig = NULL;
  args_info->port_arg = 59997;
  args_info->port_orig = NULL;
  args_info->remotehost_arg = gengetopt_strdup ("localhost");
  args_info->remotehost_orig = NULL;
  args_info->remoteport_arg = 59999;
  args_info->remoteport_orig = NULL;
  args_info->method_arg = gengetopt_strdup ("TCP");
  args_info->method_orig = NULL;
  args_info->verbose_arg = 3;
  args_info->verbose_orig = NULL;
  args_info->contact_arg = NULL;
  args_info->contact_orig = NULL;
  args_info->passive_flag = 0;
  args_info->nclient_orig = NULL;
  args_info->isnative_flag = 0;
  args_info->remotelist_arg = NULL;
  args_info->remotelist_orig = NULL;
  args_info->attrlist_arg = NULL;
  args_info->attrlist_orig = NULL;
  
}

static
void init_args_info(struct icee_args_info *args_info)
{


  args_info->help_help = icee_args_info_help[0] ;
  args_info->version_help = icee_args_info_help[1] ;
  args_info->client_help = icee_args_info_help[2] ;
  args_info->writemethod_help = icee_args_info_help[3] ;
  args_info->readmethod_help = icee_args_info_help[4] ;
  args_info->len_help = icee_args_info_help[5] ;
  args_info->timeout_help = icee_args_info_help[6] ;
  args_info->host_help = icee_args_info_help[7] ;
  args_info->port_help = icee_args_info_help[8] ;
  args_info->remotehost_help = icee_args_info_help[9] ;
  args_info->remoteport_help = icee_args_info_help[10] ;
  args_info->method_help = icee_args_info_help[11] ;
  args_info->verbose_help = icee_args_info_help[12] ;
  args_info->contact_help = icee_args_info_help[13] ;
  args_info->passive_help = icee_args_info_help[14] ;
  args_info->nclient_help = icee_args_info_help[15] ;
  args_info->isnative_help = icee_args_info_help[16] ;
  args_info->remotelist_help = icee_args_info_help[17] ;
  args_info->attrlist_help = icee_args_info_help[18] ;
  
}

void
icee_cmdline_parser_print_version (void)
{
  printf ("%s %s\n",
     (strlen(ICEE_CMDLINE_PARSER_PACKAGE_NAME) ? ICEE_CMDLINE_PARSER_PACKAGE_NAME : ICEE_CMDLINE_PARSER_PACKAGE),
     ICEE_CMDLINE_PARSER_VERSION);
}

static void print_help_common(void) {
  icee_cmdline_parser_print_version ();

  if (strlen(icee_args_info_purpose) > 0)
    printf("\n%s\n", icee_args_info_purpose);

  if (strlen(icee_args_info_usage) > 0)
    printf("\n%s\n", icee_args_info_usage);

  printf("\n");

  if (strlen(icee_args_info_description) > 0)
    printf("%s\n\n", icee_args_info_description);
}

void
icee_cmdline_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (icee_args_info_help[i])
    printf("%s\n", icee_args_info_help[i++]);
}

void
icee_cmdline_parser_init (struct icee_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);
}

void
icee_cmdline_parser_params_init(struct icee_cmdline_parser_params *params)
{
  if (params)
    { 
      params->override = 0;
      params->initialize = 1;
      params->check_required = 1;
      params->check_ambiguity = 0;
      params->print_errors = 1;
    }
}

struct icee_cmdline_parser_params *
icee_cmdline_parser_params_create(void)
{
  struct icee_cmdline_parser_params *params = 
    (struct icee_cmdline_parser_params *)malloc(sizeof(struct icee_cmdline_parser_params));
  icee_cmdline_parser_params_init(params);  
  return params;
}

static void
free_string_field (char **s)
{
  if (*s)
    {
      free (*s);
      *s = 0;
    }
}


static void
icee_cmdline_parser_release (struct icee_args_info *args_info)
{

  free_string_field (&(args_info->writemethod_arg));
  free_string_field (&(args_info->writemethod_orig));
  free_string_field (&(args_info->readmethod_arg));
  free_string_field (&(args_info->readmethod_orig));
  free_string_field (&(args_info->len_orig));
  free_string_field (&(args_info->timeout_orig));
  free_string_field (&(args_info->host_arg));
  free_string_field (&(args_info->host_orig));
  free_string_field (&(args_info->port_orig));
  free_string_field (&(args_info->remotehost_arg));
  free_string_field (&(args_info->remotehost_orig));
  free_string_field (&(args_info->remoteport_orig));
  free_string_field (&(args_info->method_arg));
  free_string_field (&(args_info->method_orig));
  free_string_field (&(args_info->verbose_orig));
  free_string_field (&(args_info->contact_arg));
  free_string_field (&(args_info->contact_orig));
  free_string_field (&(args_info->nclient_orig));
  free_string_field (&(args_info->remotelist_arg));
  free_string_field (&(args_info->remotelist_orig));
  free_string_field (&(args_info->attrlist_arg));
  free_string_field (&(args_info->attrlist_orig));
  
  

  clear_given (args_info);
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, const char *values[])
{
  FIX_UNUSED (values);
  if (arg) {
    fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}


int
icee_cmdline_parser_dump(FILE *outfile, struct icee_args_info *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", ICEE_CMDLINE_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->client_given)
    write_into_file(outfile, "client", 0, 0 );
  if (args_info->writemethod_given)
    write_into_file(outfile, "writemethod", args_info->writemethod_orig, 0);
  if (args_info->readmethod_given)
    write_into_file(outfile, "readmethod", args_info->readmethod_orig, 0);
  if (args_info->len_given)
    write_into_file(outfile, "len", args_info->len_orig, 0);
  if (args_info->timeout_given)
    write_into_file(outfile, "timeout", args_info->timeout_orig, 0);
  if (args_info->host_given)
    write_into_file(outfile, "host", args_info->host_orig, 0);
  if (args_info->port_given)
    write_into_file(outfile, "port", args_info->port_orig, 0);
  if (args_info->remotehost_given)
    write_into_file(outfile, "remotehost", args_info->remotehost_orig, 0);
  if (args_info->remoteport_given)
    write_into_file(outfile, "remoteport", args_info->remoteport_orig, 0);
  if (args_info->method_given)
    write_into_file(outfile, "method", args_info->method_orig, 0);
  if (args_info->verbose_given)
    write_into_file(outfile, "verbose", args_info->verbose_orig, 0);
  if (args_info->contact_given)
    write_into_file(outfile, "contact", args_info->contact_orig, 0);
  if (args_info->passive_given)
    write_into_file(outfile, "passive", 0, 0 );
  if (args_info->nclient_given)
    write_into_file(outfile, "nclient", args_info->nclient_orig, 0);
  if (args_info->isnative_given)
    write_into_file(outfile, "isnative", 0, 0 );
  if (args_info->remotelist_given)
    write_into_file(outfile, "remotelist", args_info->remotelist_orig, 0);
  if (args_info->attrlist_given)
    write_into_file(outfile, "attrlist", args_info->attrlist_orig, 0);
  

  i = EXIT_SUCCESS;
  return i;
}

int
icee_cmdline_parser_file_save(const char *filename, struct icee_args_info *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", ICEE_CMDLINE_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = icee_cmdline_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
icee_cmdline_parser_free (struct icee_args_info *args_info)
{
  icee_cmdline_parser_release (args_info);
}

/** @brief replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = 0;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
icee_cmdline_parser (int argc, char **argv, struct icee_args_info *args_info)
{
  return icee_cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
icee_cmdline_parser_ext (int argc, char **argv, struct icee_args_info *args_info,
                   struct icee_cmdline_parser_params *params)
{
  int result;
  result = icee_cmdline_parser_internal (argc, argv, args_info, params, 0);

  if (result == EXIT_FAILURE)
    {
      icee_cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
icee_cmdline_parser2 (int argc, char **argv, struct icee_args_info *args_info, int override, int initialize, int check_required)
{
  int result;
  struct icee_cmdline_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = icee_cmdline_parser_internal (argc, argv, args_info, &params, 0);

  if (result == EXIT_FAILURE)
    {
      icee_cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
icee_cmdline_parser_required (struct icee_args_info *args_info, const char *prog_name)
{
  FIX_UNUSED (args_info);
  FIX_UNUSED (prog_name);
  return EXIT_SUCCESS;
}


static char *package_name = 0;

/**
 * @brief updates an option
 * @param field the generic pointer to the field to update
 * @param orig_field the pointer to the orig field
 * @param field_given the pointer to the number of occurrence of this option
 * @param prev_given the pointer to the number of occurrence already seen
 * @param value the argument for this option (if null no arg was specified)
 * @param possible_values the possible values for this option (if specified)
 * @param default_value the default value (in case the option only accepts fixed values)
 * @param arg_type the type of this option
 * @param check_ambiguity @see icee_cmdline_parser_params.check_ambiguity
 * @param override @see icee_cmdline_parser_params.override
 * @param no_free whether to free a possible previous value
 * @param multiple_option whether this is a multiple option
 * @param long_opt the corresponding long option
 * @param short_opt the corresponding short option (or '-' if none)
 * @param additional_error possible further error specification
 */
static
int update_arg(void *field, char **orig_field,
               unsigned int *field_given, unsigned int *prev_given, 
               char *value, const char *possible_values[],
               const char *default_value,
               icee_cmdline_parser_arg_type arg_type,
               int check_ambiguity, int override,
               int no_free, int multiple_option,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  char *stop_char = 0;
  const char *val = value;
  int found;
  char **string_field;
  FIX_UNUSED (field);

  stop_char = 0;
  found = 0;

  if (!multiple_option && prev_given && (*prev_given || (check_ambiguity && *field_given)))
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: `--%s' (`-%c') option given more than once%s\n", 
               package_name, long_opt, short_opt,
               (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: `--%s' option given more than once%s\n", 
               package_name, long_opt,
               (additional_error ? additional_error : ""));
      return 1; /* failure */
    }

  FIX_UNUSED (default_value);
    
  if (field_given && *field_given && ! override)
    return 0;
  if (prev_given)
    (*prev_given)++;
  if (field_given)
    (*field_given)++;
  if (possible_values)
    val = possible_values[found];

  switch(arg_type) {
  case ARG_FLAG:
    *((int *)field) = !*((int *)field);
    break;
  case ARG_INT:
    if (val) *((int *)field) = strtol (val, &stop_char, 0);
    break;
  case ARG_FLOAT:
    if (val) *((float *)field) = (float)strtod (val, &stop_char);
    break;
  case ARG_STRING:
    if (val) {
      string_field = (char **)field;
      if (!no_free && *string_field)
        free (*string_field); /* free previous string */
      *string_field = gengetopt_strdup (val);
    }
    break;
  default:
    break;
  };

  /* check numeric conversion */
  switch(arg_type) {
  case ARG_INT:
  case ARG_FLOAT:
    if (val && !(stop_char && *stop_char == '\0')) {
      fprintf(stderr, "%s: invalid numeric value: %s\n", package_name, val);
      return 1; /* failure */
    }
    break;
  default:
    ;
  };

  /* store the original value */
  switch(arg_type) {
  case ARG_NO:
  case ARG_FLAG:
    break;
  default:
    if (value && orig_field) {
      if (no_free) {
        *orig_field = value;
      } else {
        if (*orig_field)
          free (*orig_field); /* free previous string */
        *orig_field = gengetopt_strdup (value);
      }
    }
  };

  return 0; /* OK */
}


int
icee_cmdline_parser_internal (
  int argc, char **argv, struct icee_args_info *args_info,
                        struct icee_cmdline_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  int error = 0;
  struct icee_args_info local_args_info;
  
  int override;
  int initialize;
  int check_required;
  int check_ambiguity;
  
  package_name = argv[0];
  
  override = params->override;
  initialize = params->initialize;
  check_required = params->check_required;
  check_ambiguity = params->check_ambiguity;

  if (initialize)
    icee_cmdline_parser_init (args_info);

  icee_cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "client",	0, NULL, 'c' },
        { "writemethod",	1, NULL, 'w' },
        { "readmethod",	1, NULL, 'r' },
        { "len",	1, NULL, 'n' },
        { "timeout",	1, NULL, 0 },
        { "host",	1, NULL, 0 },
        { "port",	1, NULL, 'p' },
        { "remotehost",	1, NULL, 's' },
        { "remoteport",	1, NULL, 't' },
        { "method",	1, NULL, 'T' },
        { "verbose",	1, NULL, 'v' },
        { "contact",	1, NULL, 'e' },
        { "passive",	0, NULL, 0 },
        { "nclient",	1, NULL, 0 },
        { "isnative",	0, NULL, 0 },
        { "remotelist",	1, NULL, 0 },
        { "attrlist",	1, NULL, 0 },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hVcw:r:n:p:s:t:T:v:e:", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          icee_cmdline_parser_print_help ();
          icee_cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          icee_cmdline_parser_print_version ();
          icee_cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'c':	/* Client mode.  */
        
        
          if (update_arg((void *)&(args_info->client_flag), 0, &(args_info->client_given),
              &(local_args_info.client_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "client", 'c',
              additional_error))
            goto failure;
        
          break;
        case 'w':	/* ADIOS write method.  */
        
        
          if (update_arg( (void *)&(args_info->writemethod_arg), 
               &(args_info->writemethod_orig), &(args_info->writemethod_given),
              &(local_args_info.writemethod_given), optarg, 0, "POSIX1", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "writemethod", 'w',
              additional_error))
            goto failure;
        
          break;
        case 'r':	/* ADIOS read method.  */
        
        
          if (update_arg( (void *)&(args_info->readmethod_arg), 
               &(args_info->readmethod_orig), &(args_info->readmethod_given),
              &(local_args_info.readmethod_given), optarg, 0, "BP", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "readmethod", 'r',
              additional_error))
            goto failure;
        
          break;
        case 'n':	/* array length.  */
        
        
          if (update_arg( (void *)&(args_info->len_arg), 
               &(args_info->len_orig), &(args_info->len_given),
              &(local_args_info.len_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "len", 'n',
              additional_error))
            goto failure;
        
          break;
        case 'p':	/* local port.  */
        
        
          if (update_arg( (void *)&(args_info->port_arg), 
               &(args_info->port_orig), &(args_info->port_given),
              &(local_args_info.port_given), optarg, 0, "59997", ARG_INT,
              check_ambiguity, override, 0, 0,
              "port", 'p',
              additional_error))
            goto failure;
        
          break;
        case 's':	/* remote hostname.  */
        
        
          if (update_arg( (void *)&(args_info->remotehost_arg), 
               &(args_info->remotehost_orig), &(args_info->remotehost_given),
              &(local_args_info.remotehost_given), optarg, 0, "localhost", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "remotehost", 's',
              additional_error))
            goto failure;
        
          break;
        case 't':	/* remote port.  */
        
        
          if (update_arg( (void *)&(args_info->remoteport_arg), 
               &(args_info->remoteport_orig), &(args_info->remoteport_given),
              &(local_args_info.remoteport_given), optarg, 0, "59999", ARG_INT,
              check_ambiguity, override, 0, 0,
              "remoteport", 't',
              additional_error))
            goto failure;
        
          break;
        case 'T':	/* EVPath method.  */
        
        
          if (update_arg( (void *)&(args_info->method_arg), 
               &(args_info->method_orig), &(args_info->method_given),
              &(local_args_info.method_given), optarg, 0, "TCP", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "method", 'T',
              additional_error))
            goto failure;
        
          break;
        case 'v':	/* verbosity level.  */
        
        
          if (update_arg( (void *)&(args_info->verbose_arg), 
               &(args_info->verbose_orig), &(args_info->verbose_given),
              &(local_args_info.verbose_given), optarg, 0, "3", ARG_INT,
              check_ambiguity, override, 0, 0,
              "verbose", 'v',
              additional_error))
            goto failure;
        
          break;
        case 'e':	/* EVPath contact string.  */
        
        
          if (update_arg( (void *)&(args_info->contact_arg), 
               &(args_info->contact_orig), &(args_info->contact_given),
              &(local_args_info.contact_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "contact", 'e',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
          /* Timeout.  */
          if (strcmp (long_options[option_index].name, "timeout") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->timeout_arg), 
                 &(args_info->timeout_orig), &(args_info->timeout_given),
                &(local_args_info.timeout_given), optarg, 0, 0, ARG_FLOAT,
                check_ambiguity, override, 0, 0,
                "timeout", '-',
                additional_error))
              goto failure;
          
          }
          /* local hostname.  */
          else if (strcmp (long_options[option_index].name, "host") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->host_arg), 
                 &(args_info->host_orig), &(args_info->host_given),
                &(local_args_info.host_given), optarg, 0, "localhost", ARG_STRING,
                check_ambiguity, override, 0, 0,
                "host", '-',
                additional_error))
              goto failure;
          
          }
          /* enable passive mode.  */
          else if (strcmp (long_options[option_index].name, "passive") == 0)
          {
          
          
            if (update_arg((void *)&(args_info->passive_flag), 0, &(args_info->passive_given),
                &(local_args_info.passive_given), optarg, 0, 0, ARG_FLAG,
                check_ambiguity, override, 1, 0, "passive", '-',
                additional_error))
              goto failure;
          
          }
          /* maximum number of clients.  */
          else if (strcmp (long_options[option_index].name, "nclient") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->nclient_arg), 
                 &(args_info->nclient_orig), &(args_info->nclient_given),
                &(local_args_info.nclient_given), optarg, 0, 0, ARG_INT,
                check_ambiguity, override, 0, 0,
                "nclient", '-',
                additional_error))
              goto failure;
          
          }
          /* use native contact.  */
          else if (strcmp (long_options[option_index].name, "isnative") == 0)
          {
          
          
            if (update_arg((void *)&(args_info->isnative_flag), 0, &(args_info->isnative_given),
                &(local_args_info.isnative_given), optarg, 0, 0, ARG_FLAG,
                check_ambiguity, override, 1, 0, "isnative", '-',
                additional_error))
              goto failure;
          
          }
          /* remote list.  */
          else if (strcmp (long_options[option_index].name, "remotelist") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->remotelist_arg), 
                 &(args_info->remotelist_orig), &(args_info->remotelist_given),
                &(local_args_info.remotelist_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "remotelist", '-',
                additional_error))
              goto failure;
          
          }
          /* attr list.  */
          else if (strcmp (long_options[option_index].name, "attrlist") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->attrlist_arg), 
                 &(args_info->attrlist_orig), &(args_info->attrlist_given),
                &(local_args_info.attrlist_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "attrlist", '-',
                additional_error))
              goto failure;
          
          }
          
          break;
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", ICEE_CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */




  icee_cmdline_parser_release (&local_args_info);

  if ( error )
    return (EXIT_FAILURE);

  return 0;

failure:
  
  icee_cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}
