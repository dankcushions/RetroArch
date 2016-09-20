/* RetroArch - A frontend for libretro.
* Copyright (C) 2011-2016 - Daniel De Matteis
*
* RetroArch is free software: you can redistribute it and/or modify it under the terms
* of the GNU General Public License as published by the Free Software Found-
* ation, either version 3 of the License, or (at your option) any later version.
*
* RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with RetroArch.
* If not, see <http://www.gnu.org/licenses/>.
*/

#if defined(HAVE_CG) || defined(HAVE_HLSL) || defined(HAVE_GLSL)
#define HAVE_SHADERS
#endif

#if defined(HAVE_ZLIB) || defined(HAVE_7ZIP)
#define HAVE_COMPRESSION
#endif

#if defined(_MSC_VER)
#include <compat/posix_string.h>
#endif

#ifdef WANT_GLSLANG
#ifdef HAVE_VULKAN
#include "../deps/glslang/glslang.cpp"
#include "../deps/glslang/glslang_tab.cpp"
#include "../deps/glslang/glslang/SPIRV/SpvBuilder.cpp"
#include "../deps/glslang/glslang/SPIRV/SPVRemapper.cpp"
#include "../deps/glslang/glslang/SPIRV/InReadableOrder.cpp"
#include "../deps/glslang/glslang/SPIRV/doc.cpp"
#include "../deps/glslang/glslang/SPIRV/GlslangToSpv.cpp"
#include "../deps/glslang/glslang/SPIRV/disassemble.cpp"
#include "../deps/glslang/glslang/SPIRV/logger.cpp"
#include "../deps/glslang/glslang/glslang/GenericCodeGen/Link.cpp"
#include "../deps/glslang/glslang/glslang/GenericCodeGen/CodeGen.cpp"
#include "../deps/glslang/glslang/hlsl/hlslGrammar.cpp"
#include "../deps/glslang/glslang/hlsl/hlslOpMap.cpp"
#include "../deps/glslang/glslang/hlsl/hlslTokenStream.cpp"
#include "../deps/glslang/glslang/hlsl/hlslScanContext.cpp"
#include "../deps/glslang/glslang/hlsl/hlslParseHelper.cpp"
#include "../deps/glslang/glslang/hlsl/hlslParseables.cpp"
#include "../deps/glslang/glslang/glslang/GenericCodeGen/CodeGen.cpp"
#include "../deps/glslang/glslang/OGLCompilersDLL/InitializeDll.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/Intermediate.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/propagateNoContraction.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/Versions.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/RemoveTree.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/limits.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/intermOut.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/Initialize.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/SymbolTable.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/parseConst.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/ParseHelper.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/ShaderLang.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/IntermTraverse.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/InfoSink.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/Constant.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/Scan.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/reflection.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/linkValidate.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/PoolAlloc.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/preprocessor/PpAtom.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/preprocessor/PpContext.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/preprocessor/PpMemory.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/preprocessor/PpTokens.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/preprocessor/PpScanner.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/preprocessor/PpSymbols.cpp"
#include "../deps/glslang/glslang/glslang/MachineIndependent/preprocessor/Pp.cpp"

#ifdef _WIN32
#include "../deps/glslang/glslang/glslang/OSDependent/Windows/ossource.cpp"
#endif

#if defined(__linux__) && !defined(ANDROID)
#include "../deps/glslang/glslang/glslang/OSDependent/Unix/ossource.cpp"
#endif
#endif
#endif

#ifdef GLSL_OPTIMIZER
#include "../gfx/drivers_shader/glsl_optimizer.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_copy_propagation_elements.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_constant_folding.cpp"
#include "../deps/glsl-optimizer/src/glsl/s_expression.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_constant_propagation.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_ubo_reference.cpp"
#include "../deps/glsl-optimizer/src/glsl/glsl_parser.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_variable_index_to_cond_assign.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_function_can_inline.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_unused_structs.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_print_visitor.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_constant_expression.cpp"
#include "../deps/glsl-optimizer/src/glsl/glsl_parser_extras.cpp"
#include "../deps/glsl-optimizer/src/glsl/hir_field_selection.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_noise.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_dead_code_local.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_if_simplification.cpp"
#include "../deps/glsl-optimizer/src/glsl/builtin_types.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_vector_insert.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_packing_builtins.cpp"
#include "../deps/glsl-optimizer/src/glsl/ast_to_hir.cpp"
#include "../deps/glsl-optimizer/src/glsl/link_uniforms.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_vector.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_equals.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_builder.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_redundant_jumps.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_validate.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_vectorize.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_rvalue_visitor.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_expression_flattening.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_tree_grafting.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_noop_swizzle.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_clip_distance.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_import_prototypes.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_vertex_id.cpp"
#include "../deps/glsl-optimizer/src/glsl/link_atomics.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_rebalance_tree.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_minmax.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_dead_builtin_variables.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_vec_index_to_swizzle.cpp"
#include "../deps/glsl-optimizer/src/glsl/loop_unroll.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_dead_code.cpp"
#include "../deps/glsl-optimizer/src/glsl/ast_expr.cpp"
#include "../deps/glsl-optimizer/src/glsl/loop_controls.cpp"
#include "../deps/glsl-optimizer/src/glsl/linker.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_function_inlining.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_mat_op_to_vec.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_function_detect_recursion.cpp"
#include "../deps/glsl-optimizer/src/glsl/link_uniform_blocks.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_swizzle_swizzle.cpp"
#include "../deps/glsl-optimizer/src/glsl/link_functions.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_output_reads.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_function.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_named_interface_blocks.cpp"
#include "../deps/glsl-optimizer/src/glsl/loop_analysis.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_stats.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_algebraic.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_variable_refcount.cpp"
#include "../deps/glsl-optimizer/src/glsl/ast_array_index.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_hierarchical_visitor.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_cse.cpp"
#include "../deps/glsl-optimizer/src/glsl/builtin_variables.cpp"
#include "../deps/glsl-optimizer/src/glsl/ast_type.cpp"
#include "../deps/glsl-optimizer/src/glsl/link_varyings.cpp"
#include "../deps/glsl-optimizer/src/glsl/glsl_types.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_dead_builtin_varyings.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_print_glsl_visitor.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_print_metal_visitor.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_offset_array.cpp"
#include "../deps/glsl-optimizer/src/glsl/link_uniform_initializers.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_constant_variable.cpp"
#include "../deps/glsl-optimizer/src/glsl/builtin_functions.cpp"
#include "../deps/glsl-optimizer/src/glsl/ast_function.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_dead_functions.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_basic_block.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_if_to_cond_assign.cpp"
#include "../deps/glsl-optimizer/src/glsl/link_interface_blocks.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_structure_splitting.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_vec_index_to_cond_assign.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_hv_accept.cpp"
#include "../deps/glsl-optimizer/src/glsl/ir_clone.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_flip_matrices.cpp"
#include "../deps/glsl-optimizer/src/glsl/standalone_scaffolding.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_array_splitting.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_discard.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_packed_varyings.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_discard_flow.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_copy_propagation.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_jumps.cpp"
#include "../deps/glsl-optimizer/src/glsl/lower_instructions.cpp"
#include "../deps/glsl-optimizer/src/glsl/glsl_lexer.cpp"
#include "../deps/glsl-optimizer/src/glsl/link_uniform_block_active_visitor.cpp"
#include "../deps/glsl-optimizer/src/glsl/opt_flatten_nested_if_blocks.cpp"
#include "../deps/glsl-optimizer/src/glsl/glsl_symbol_table.cpp"
#endif

/*============================================================
AUDIO
============================================================ */
#ifdef HAVE_XAUDIO
#include "../audio/drivers/xaudio.cpp"
#endif


/*============================================================
 KEYBOARD EVENT
 ============================================================ */
#if defined(_WIN32) && !defined(_XBOX)
#include "../input/drivers_keyboard/keyboard_event_win32.cpp"
#endif

/*============================================================
UI COMMON CONTEXT
============================================================ */
#if defined(_WIN32) && !defined(_XBOX)
#include "../gfx/common/win32_common.cpp"

#if defined(HAVE_OPENGL) || defined(HAVE_VULKAN)
#include "../gfx/drivers_context/wgl_ctx.cpp"
#endif

#if defined(HAVE_FFMPEG)
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES3)
#include "../cores/libretro-ffmpeg/fft/fft.cpp"
#endif
#endif

#endif


/*============================================================
MENU
============================================================ */
#ifdef HAVE_XUI
#include "../menu/drivers/xui.cpp"
#endif

#if defined(HAVE_D3D)
#include "../menu/drivers_display/menu_display_d3d.cpp"
#endif

/*============================================================
VIDEO CONTEXT
============================================================ */

#if defined(HAVE_D3D)
#include "../gfx/drivers_context/d3d_ctx.cpp"
#endif

/*============================================================
UI
============================================================ */

#if defined(_WIN32) && !defined(_XBOX)
#include "../ui/drivers/win32/ui_win32_window.cpp"
#endif

#if defined(HAVE_QT)
#include "../ui/drivers/ui_qt.cpp"

#include "../ui/drivers/qt/ui_qt_window.cpp"
#include "../ui/drivers/qt/ui_qt_browser_window.cpp"
#include "../ui/drivers/qt/ui_qt_msg_window.cpp"
#include "../ui/drivers/qt/ui_qt_application.cpp"
#endif

/*============================================================
VIDEO DRIVER
============================================================ */
#ifdef _XBOX
#include "../frontend/drivers/platform_xdk.cpp"
#endif

#if defined(HAVE_D3D)
#include "../gfx/common/d3d_common.cpp"
#include "../gfx/drivers/d3d.cpp"
#ifdef _XBOX
#include "../gfx/drivers/d3d_renderchains/render_chain_xdk.cpp"
#endif
#ifdef HAVE_CG
#include "../gfx/drivers/d3d_renderchains/render_chain_cg.cpp"
#endif
#endif

#ifdef HAVE_VULKAN
#include "../gfx/drivers_shader/shader_vulkan.cpp"
#include "../gfx/drivers_shader/glslang_util.cpp"
#include "../gfx/drivers_shader/slang_reflection.cpp"
#include "../deps/SPIRV-Cross/spirv_cross.cpp"
#endif

#ifdef GLSL_OPTIMIZER
#include "../gfx/drivers_shader/glsl_optimizer.cpp"
#endif

/*============================================================
FONTS
============================================================ */

#if defined(HAVE_D3D9) && !defined(_XBOX)
#include "../gfx/drivers_font/d3d_w32_font.cpp"
#endif

#if defined(_XBOX360)
#include "../gfx/drivers_font/xdk360_fonts.cpp"
#endif
