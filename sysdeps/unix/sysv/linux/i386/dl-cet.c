/* Linux/i386 CET initializers function.
   Copyright (C) 2017 Free Software Foundation, Inc.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifdef SHARED
# define LINKAGE static inline
# define _dl_cet_init cet_init
# include <sysdeps/unix/sysv/linux/x86/dl-cet.c>
# undef _dl_cet_init

void
internal_function
_dl_cet_init (struct link_map *main_map, int argc, char **argv, char **env)
{
  cet_init (main_map, argc, argv, env);

  if ((GL(dl_x86_feature_1) & GNU_PROPERTY_X86_FEATURE_1_SHSTK))
    {
      /* Replace _dl_runtime_resolve with _dl_runtime_resolve_shstk if
	 SHSTK is enabled.  */
      extern void _dl_runtime_resolve (Elf32_Word) attribute_hidden;
      extern void _dl_runtime_resolve_shstk (Elf32_Word) attribute_hidden;
      unsigned int i;
      struct link_map *l;
      Elf32_Addr *got;

      if (main_map->l_info[DT_JMPREL])
	{
	  got = (Elf32_Addr *) D_PTR (main_map, l_info[DT_PLTGOT]);
	  if (got[2] == (Elf32_Addr) &_dl_runtime_resolve)
	    got[2] = (Elf32_Addr) &_dl_runtime_resolve_shstk;
	}

      i = main_map->l_searchlist.r_nlist;
      while (i-- > 0)
	{
	  l = main_map->l_initfini[i];
	  if (l->l_info[DT_JMPREL])
	    {
	      got = (Elf32_Addr *) D_PTR (l, l_info[DT_PLTGOT]);
	      if (got[2] == (Elf32_Addr) &_dl_runtime_resolve)
		got[2] = (Elf32_Addr) &_dl_runtime_resolve_shstk;
	    }
	}
    }
}
#endif