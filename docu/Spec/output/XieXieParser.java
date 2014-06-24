// $ANTLR 3.5 D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g 2014-06-10 20:58:55

import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import org.antlr.runtime.debug.*;
import java.io.IOException;
@SuppressWarnings("all")
public class XieXieParser extends DebugParser {
	public static final String[] tokenNames = new String[] {
		"<invalid>", "<EOR>", "<DOWN>", "<UP>", "BIN_LITERAL", "BLOCK_COMMENT", 
		"BOOL_LITERAL", "BOOL_TYPE_DENOTER", "BinDigit", "CHAR_LITERAL", "DEC_LITERAL", 
		"DecDigit", "EscapeCharacter", "FLOAT_LITERAL", "FLOAT_TYPE_DENOTER", 
		"HEX_LITERAL", "HexDigit", "IDENT", "INT_LITERAL", "INT_TYPE_DENOTER", 
		"LINE_COMMENT", "Letter", "NULL_LITERAL", "OCT_LITERAL", "OctDigit", "SUPER_LITERAL", 
		"THIS_LITERAL", "WS", "'!='", "'%'", "'%='", "'&'", "'&='", "'('", "')'", 
		"'*'", "'*='", "'+'", "'++'", "'+='", "','", "'-'", "'--'", "'-='", "'->'", 
		"'.'", "'..'", "'/'", "'/='", "':'", "':='", "';'", "'<'", "'<<'", "'<<='", 
		"'<='", "'='", "'>'", "'>='", "'>>'", "'>>='", "'[['", "']]'", "'^'", 
		"'^='", "'and'", "'break'", "'case'", "'class'", "'continue'", "'default'", 
		"'do'", "'else'", "'enum'", "'extern'", "'flags'", "'for'", "'foreach'", 
		"'forever'", "'if'", "'not'", "'or'", "'return'", "'static'", "'switch'", 
		"'while'", "'{'", "'|'", "'|='", "'}'", "'~'"
	};
	public static final int EOF=-1;
	public static final int T__28=28;
	public static final int T__29=29;
	public static final int T__30=30;
	public static final int T__31=31;
	public static final int T__32=32;
	public static final int T__33=33;
	public static final int T__34=34;
	public static final int T__35=35;
	public static final int T__36=36;
	public static final int T__37=37;
	public static final int T__38=38;
	public static final int T__39=39;
	public static final int T__40=40;
	public static final int T__41=41;
	public static final int T__42=42;
	public static final int T__43=43;
	public static final int T__44=44;
	public static final int T__45=45;
	public static final int T__46=46;
	public static final int T__47=47;
	public static final int T__48=48;
	public static final int T__49=49;
	public static final int T__50=50;
	public static final int T__51=51;
	public static final int T__52=52;
	public static final int T__53=53;
	public static final int T__54=54;
	public static final int T__55=55;
	public static final int T__56=56;
	public static final int T__57=57;
	public static final int T__58=58;
	public static final int T__59=59;
	public static final int T__60=60;
	public static final int T__61=61;
	public static final int T__62=62;
	public static final int T__63=63;
	public static final int T__64=64;
	public static final int T__65=65;
	public static final int T__66=66;
	public static final int T__67=67;
	public static final int T__68=68;
	public static final int T__69=69;
	public static final int T__70=70;
	public static final int T__71=71;
	public static final int T__72=72;
	public static final int T__73=73;
	public static final int T__74=74;
	public static final int T__75=75;
	public static final int T__76=76;
	public static final int T__77=77;
	public static final int T__78=78;
	public static final int T__79=79;
	public static final int T__80=80;
	public static final int T__81=81;
	public static final int T__82=82;
	public static final int T__83=83;
	public static final int T__84=84;
	public static final int T__85=85;
	public static final int T__86=86;
	public static final int T__87=87;
	public static final int T__88=88;
	public static final int T__89=89;
	public static final int T__90=90;
	public static final int BIN_LITERAL=4;
	public static final int BLOCK_COMMENT=5;
	public static final int BOOL_LITERAL=6;
	public static final int BOOL_TYPE_DENOTER=7;
	public static final int BinDigit=8;
	public static final int CHAR_LITERAL=9;
	public static final int DEC_LITERAL=10;
	public static final int DecDigit=11;
	public static final int EscapeCharacter=12;
	public static final int FLOAT_LITERAL=13;
	public static final int FLOAT_TYPE_DENOTER=14;
	public static final int HEX_LITERAL=15;
	public static final int HexDigit=16;
	public static final int IDENT=17;
	public static final int INT_LITERAL=18;
	public static final int INT_TYPE_DENOTER=19;
	public static final int LINE_COMMENT=20;
	public static final int Letter=21;
	public static final int NULL_LITERAL=22;
	public static final int OCT_LITERAL=23;
	public static final int OctDigit=24;
	public static final int SUPER_LITERAL=25;
	public static final int THIS_LITERAL=26;
	public static final int WS=27;

	// delegates
	public Parser[] getDelegates() {
		return new Parser[] {};
	}

	// delegators


	public static final String[] ruleNames = new String[] {
		"invalidRule", "synpred40_XieXie", "synpred27_XieXie", "expr_list", "synpred5_XieXie", 
		"synpred23_XieXie", "synpred64_XieXie", "expr_pattern", "synpred31_XieXie", 
		"synpred10_XieXie", "synpred15_XieXie", "synpred48_XieXie", "synpred71_XieXie", 
		"div_expr", "enum_entry_list", "enum_entry", "synpred18_XieXie", "synpred30_XieXie", 
		"synpred76_XieXie", "synpred96_XieXie", "enum_decl_stmnt", "do_while_stmnt", 
		"synpred94_XieXie", "enum_body", "bitwise_or_expr", "variable_ident", 
		"synpred7_XieXie", "synpred13_XieXie", "pattern", "synpred22_XieXie", 
		"synpred80_XieXie", "attribute_argument_list", "mul_expr", "program", 
		"shift_expr", "sub_expr", "synpred33_XieXie", "synpred54_XieXie", "synpred47_XieXie", 
		"synpred24_XieXie", "synpred72_XieXie", "synpred3_XieXie", "attribute_list", 
		"switch_stmnt", "unary_expr", "synpred35_XieXie", "decl_stmnt_list", "variable_decl_stmnt", 
		"synpred53_XieXie", "for_stmnt", "enum_entry_name", "type_inheritance", 
		"synpred34_XieXie", "synpred68_XieXie", "for_range_stmnt", "flags_body", 
		"buildin_type_denoter", "parameter_list", "synpred4_XieXie", "synpred60_XieXie", 
		"function_modifier", "bitwise_and_expr", "case_item_list", "continue_stmnt", 
		"synpred87_XieXie", "synpred39_XieXie", "synpred95_XieXie", "synpred58_XieXie", 
		"synpred67_XieXie", "relation_expr", "synpred92_XieXie", "synpred29_XieXie", 
		"stmnt", "default_label", "synpred88_XieXie", "synpred41_XieXie", "switch_case", 
		"synpred12_XieXie", "synpred84_XieXie", "synpred65_XieXie", "function_head", 
		"synpred69_XieXie", "class_body", "synpred36_XieXie", "synpred9_XieXie", 
		"synpred73_XieXie", "bitwise_xor_expr", "attribute_prefix", "synpred83_XieXie", 
		"enum_case_pattern", "flags_entry_list", "synpred25_XieXie", "synpred77_XieXie", 
		"function_decl_stmnt", "synpred85_XieXie", "for_range", "synpred14_XieXie", 
		"variable_init", "synpred98_XieXie", "synpred90_XieXie", "flags_decl_stmnt", 
		"bracket_expr", "expr", "arithmetic_expr", "synpred86_XieXie", "synpred37_XieXie", 
		"synpred66_XieXie", "class_modifier", "if_condition", "synpred81_XieXie", 
		"else_stmnt", "logic_and_expr", "synpred28_XieXie", "synpred50_XieXie", 
		"synpred63_XieXie", "synpred17_XieXie", "case_label", "synpred26_XieXie", 
		"synpred38_XieXie", "synpred70_XieXie", "for_init", "synpred79_XieXie", 
		"for_each_stmnt", "argument", "synpred43_XieXie", "stmnt_list", "synpred91_XieXie", 
		"if_stmnt", "synpred97_XieXie", "synpred55_XieXie", "return_stmnt", "type_denoter", 
		"synpred75_XieXie", "modify_assign_stmnt", "synpred42_XieXie", "synpred57_XieXie", 
		"function_call", "flags_entry", "synpred74_XieXie", "code_block", "add_expr", 
		"attribute_argument", "synpred52_XieXie", "synpred89_XieXie", "synpred32_XieXie", 
		"enum_case_name", "synpred51_XieXie", "synpred56_XieXie", "for_each_init", 
		"assign_stmnt", "attribute", "parameter", "for_ever_stmnt", "synpred46_XieXie", 
		"cast_expr", "argument_list", "synpred20_XieXie", "synpred11_XieXie", 
		"loop_stmnt", "synpred2_XieXie", "copy_assign_stmnt", "variable_name", 
		"synpred6_XieXie", "synpred49_XieXie", "post_operator_stmnt", "synpred8_XieXie", 
		"synpred78_XieXie", "synpred45_XieXie", "synpred1_XieXie", "value_expr", 
		"synpred62_XieXie", "break_stmnt", "class_decl_stmnt", "decl_stmnt", "type_multi_inheritance", 
		"synpred44_XieXie", "class_name", "synpred19_XieXie", "case_item", "synpred61_XieXie", 
		"branch_stmnt", "synpred59_XieXie", "synpred99_XieXie", "ctrl_transfer_stmnt", 
		"custom_type_denoter", "while_stmnt", "equality_expr", "switch_case_list", 
		"synpred21_XieXie", "synpred82_XieXie", "class_decl_stmnt_list", "synpred93_XieXie", 
		"logic_or_expr", "synpred16_XieXie", "call_expr", "init_list_expr"
	};

	public static final boolean[] decisionCanBacktrack = new boolean[] {
		false, // invalid decision
		false, false, false, false, false, true, true, false, true, false, true, 
		    false, false, false, false, false, false, true, true, false, false, 
		    false, false, false, true, false, false, false, true, false, false, 
		    false, false, false, false, false, false, false, false, false, false, 
		    false, false, false, false, false, false, false, false, false, false, 
		    false, false, false, false, false, false, false, false, false, true, 
		    false, false, false, false
	};

 
	public int ruleLevel = 0;
	public int getRuleLevel() { return ruleLevel; }
	public void incRuleLevel() { ruleLevel++; }
	public void decRuleLevel() { ruleLevel--; }
	public XieXieParser(TokenStream input) {
		this(input, DebugEventSocketProxy.DEFAULT_DEBUGGER_PORT, new RecognizerSharedState());
	}
	public XieXieParser(TokenStream input, int port, RecognizerSharedState state) {
		super(input, state);
		this.state.ruleMemo = new HashMap[195+1];


		DebugEventSocketProxy proxy =
			new DebugEventSocketProxy(this, port, null);

		setDebugListener(proxy);
		try {
			proxy.handshake();
		}
		catch (IOException ioe) {
			reportError(ioe);
		}
	}

	public XieXieParser(TokenStream input, DebugEventListener dbg) {
		super(input, dbg, new RecognizerSharedState());
		this.state.ruleMemo = new HashMap[195+1];


	}

	protected boolean evalPredicate(boolean result, String predicate) {
		dbg.semanticPredicate(result, predicate);
		return result;
	}

	@Override public String[] getTokenNames() { return XieXieParser.tokenNames; }
	@Override public String getGrammarFileName() { return "D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g"; }



	// $ANTLR start "program"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:14:1: program : ( class_decl_stmnt_list )? EOF ;
	public final void program() throws  {
		int program_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "program");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(14, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 1) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:14:8: ( ( class_decl_stmnt_list )? EOF )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:14:10: ( class_decl_stmnt_list )? EOF
			{
			dbg.location(14,10);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:14:10: ( class_decl_stmnt_list )?
			int alt1=2;
			try { dbg.enterSubRule(1);
			try { dbg.enterDecision(1, decisionCanBacktrack[1]);

			int LA1_0 = input.LA(1);
			if ( (LA1_0==68||LA1_0==74) ) {
				alt1=1;
			}
			} finally {dbg.exitDecision(1);}

			switch (alt1) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:14:10: class_decl_stmnt_list
					{
					dbg.location(14,10);
					pushFollow(FOLLOW_class_decl_stmnt_list_in_program43);
					class_decl_stmnt_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(1);}
			dbg.location(14,33);
			match(input,EOF,FOLLOW_EOF_in_program46); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 1, program_StartIndex); }

		}
		dbg.location(14, 35);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "program");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "program"



	// $ANTLR start "code_block"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:16:1: code_block : '{' ( stmnt_list )? '}' ;
	public final void code_block() throws  {
		int code_block_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "code_block");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(16, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 2) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:16:11: ( '{' ( stmnt_list )? '}' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:16:13: '{' ( stmnt_list )? '}'
			{
			dbg.location(16,13);
			match(input,86,FOLLOW_86_in_code_block53); if (state.failed) return;dbg.location(16,17);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:16:17: ( stmnt_list )?
			int alt2=2;
			try { dbg.enterSubRule(2);
			try { dbg.enterDecision(2, decisionCanBacktrack[2]);

			int LA2_0 = input.LA(1);
			if ( (LA2_0==BOOL_TYPE_DENOTER||LA2_0==FLOAT_TYPE_DENOTER||LA2_0==IDENT||LA2_0==INT_TYPE_DENOTER||LA2_0==61||LA2_0==66||(LA2_0 >= 68 && LA2_0 <= 69)||LA2_0==71||(LA2_0 >= 73 && LA2_0 <= 79)||(LA2_0 >= 82 && LA2_0 <= 85)) ) {
				alt2=1;
			}
			} finally {dbg.exitDecision(2);}

			switch (alt2) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:16:17: stmnt_list
					{
					dbg.location(16,17);
					pushFollow(FOLLOW_stmnt_list_in_code_block55);
					stmnt_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(2);}
			dbg.location(16,29);
			match(input,89,FOLLOW_89_in_code_block58); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 2, code_block_StartIndex); }

		}
		dbg.location(16, 31);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "code_block");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "code_block"



	// $ANTLR start "stmnt_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:19:1: stmnt_list : ( stmnt )+ ;
	public final void stmnt_list() throws  {
		int stmnt_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "stmnt_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(19, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 3) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:19:11: ( ( stmnt )+ )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:19:16: ( stmnt )+
			{
			dbg.location(19,16);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:19:16: ( stmnt )+
			int cnt3=0;
			try { dbg.enterSubRule(3);

			loop3:
			while (true) {
				int alt3=2;
				try { dbg.enterDecision(3, decisionCanBacktrack[3]);

				int LA3_0 = input.LA(1);
				if ( (LA3_0==BOOL_TYPE_DENOTER||LA3_0==FLOAT_TYPE_DENOTER||LA3_0==IDENT||LA3_0==INT_TYPE_DENOTER||LA3_0==61||LA3_0==66||(LA3_0 >= 68 && LA3_0 <= 69)||LA3_0==71||(LA3_0 >= 73 && LA3_0 <= 79)||(LA3_0 >= 82 && LA3_0 <= 85)) ) {
					alt3=1;
				}

				} finally {dbg.exitDecision(3);}

				switch (alt3) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:19:16: stmnt
					{
					dbg.location(19,16);
					pushFollow(FOLLOW_stmnt_in_stmnt_list69);
					stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					if ( cnt3 >= 1 ) break loop3;
					if (state.backtracking>0) {state.failed=true; return;}
					EarlyExitException eee = new EarlyExitException(3, input);
					dbg.recognitionException(eee);

					throw eee;
				}
				cnt3++;
			}
			} finally {dbg.exitSubRule(3);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 3, stmnt_list_StartIndex); }

		}
		dbg.location(19, 21);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "stmnt_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "stmnt_list"



	// $ANTLR start "decl_stmnt_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:20:1: decl_stmnt_list : ( decl_stmnt )+ ;
	public final void decl_stmnt_list() throws  {
		int decl_stmnt_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "decl_stmnt_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(20, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 4) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:20:16: ( ( decl_stmnt )+ )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:20:20: ( decl_stmnt )+
			{
			dbg.location(20,20);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:20:20: ( decl_stmnt )+
			int cnt4=0;
			try { dbg.enterSubRule(4);

			loop4:
			while (true) {
				int alt4=2;
				try { dbg.enterDecision(4, decisionCanBacktrack[4]);

				int LA4_0 = input.LA(1);
				if ( (LA4_0==BOOL_TYPE_DENOTER||LA4_0==FLOAT_TYPE_DENOTER||LA4_0==IDENT||LA4_0==INT_TYPE_DENOTER||LA4_0==61||LA4_0==68||(LA4_0 >= 73 && LA4_0 <= 75)||LA4_0==83) ) {
					alt4=1;
				}

				} finally {dbg.exitDecision(4);}

				switch (alt4) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:20:20: decl_stmnt
					{
					dbg.location(20,20);
					pushFollow(FOLLOW_decl_stmnt_in_decl_stmnt_list78);
					decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					if ( cnt4 >= 1 ) break loop4;
					if (state.backtracking>0) {state.failed=true; return;}
					EarlyExitException eee = new EarlyExitException(4, input);
					dbg.recognitionException(eee);

					throw eee;
				}
				cnt4++;
			}
			} finally {dbg.exitSubRule(4);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 4, decl_stmnt_list_StartIndex); }

		}
		dbg.location(20, 30);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "decl_stmnt_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "decl_stmnt_list"



	// $ANTLR start "class_decl_stmnt_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:21:1: class_decl_stmnt_list : ( class_decl_stmnt )+ ;
	public final void class_decl_stmnt_list() throws  {
		int class_decl_stmnt_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "class_decl_stmnt_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(21, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 5) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:21:22: ( ( class_decl_stmnt )+ )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:21:24: ( class_decl_stmnt )+
			{
			dbg.location(21,24);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:21:24: ( class_decl_stmnt )+
			int cnt5=0;
			try { dbg.enterSubRule(5);

			loop5:
			while (true) {
				int alt5=2;
				try { dbg.enterDecision(5, decisionCanBacktrack[5]);

				int LA5_0 = input.LA(1);
				if ( (LA5_0==68||LA5_0==74) ) {
					alt5=1;
				}

				} finally {dbg.exitDecision(5);}

				switch (alt5) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:21:24: class_decl_stmnt
					{
					dbg.location(21,24);
					pushFollow(FOLLOW_class_decl_stmnt_in_class_decl_stmnt_list85);
					class_decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					if ( cnt5 >= 1 ) break loop5;
					if (state.backtracking>0) {state.failed=true; return;}
					EarlyExitException eee = new EarlyExitException(5, input);
					dbg.recognitionException(eee);

					throw eee;
				}
				cnt5++;
			}
			} finally {dbg.exitSubRule(5);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 5, class_decl_stmnt_list_StartIndex); }

		}
		dbg.location(21, 40);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "class_decl_stmnt_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "class_decl_stmnt_list"



	// $ANTLR start "stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:23:1: stmnt : ( decl_stmnt | branch_stmnt | loop_stmnt | assign_stmnt | ctrl_transfer_stmnt );
	public final void stmnt() throws  {
		int stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(23, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 6) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:23:7: ( decl_stmnt | branch_stmnt | loop_stmnt | assign_stmnt | ctrl_transfer_stmnt )
			int alt6=5;
			try { dbg.enterDecision(6, decisionCanBacktrack[6]);

			switch ( input.LA(1) ) {
			case BOOL_TYPE_DENOTER:
			case FLOAT_TYPE_DENOTER:
			case INT_TYPE_DENOTER:
			case 61:
			case 68:
			case 73:
			case 74:
			case 75:
			case 83:
				{
				alt6=1;
				}
				break;
			case IDENT:
				{
				switch ( input.LA(2) ) {
				case 45:
					{
					int LA6_19 = input.LA(3);
					if ( (synpred6_XieXie()) ) {
						alt6=1;
					}
					else if ( (synpred9_XieXie()) ) {
						alt6=4;
					}

					else {
						if (state.backtracking>0) {state.failed=true; return;}
						int nvaeMark = input.mark();
						try {
							for (int nvaeConsume = 0; nvaeConsume < 3 - 1; nvaeConsume++) {
								input.consume();
							}
							NoViableAltException nvae =
								new NoViableAltException("", 6, 19, input);
							dbg.recognitionException(nvae);
							throw nvae;
						} finally {
							input.rewind(nvaeMark);
						}
					}

					}
					break;
				case IDENT:
					{
					alt6=1;
					}
					break;
				case 30:
				case 32:
				case 36:
				case 38:
				case 39:
				case 42:
				case 43:
				case 48:
				case 50:
				case 54:
				case 60:
				case 64:
				case 88:
					{
					alt6=4;
					}
					break;
				default:
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 6, 2, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}
				}
				break;
			case 79:
			case 84:
				{
				alt6=2;
				}
				break;
			case 71:
			case 76:
			case 77:
			case 78:
			case 85:
				{
				alt6=3;
				}
				break;
			case 66:
			case 69:
			case 82:
				{
				alt6=5;
				}
				break;
			default:
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 6, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}
			} finally {dbg.exitDecision(6);}

			switch (alt6) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:23:9: decl_stmnt
					{
					dbg.location(23,9);
					pushFollow(FOLLOW_decl_stmnt_in_stmnt94);
					decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:24:5: branch_stmnt
					{
					dbg.location(24,5);
					pushFollow(FOLLOW_branch_stmnt_in_stmnt100);
					branch_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 3 :
					dbg.enterAlt(3);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:25:5: loop_stmnt
					{
					dbg.location(25,5);
					pushFollow(FOLLOW_loop_stmnt_in_stmnt106);
					loop_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 4 :
					dbg.enterAlt(4);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:26:5: assign_stmnt
					{
					dbg.location(26,5);
					pushFollow(FOLLOW_assign_stmnt_in_stmnt112);
					assign_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 5 :
					dbg.enterAlt(5);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:27:5: ctrl_transfer_stmnt
					{
					dbg.location(27,5);
					pushFollow(FOLLOW_ctrl_transfer_stmnt_in_stmnt118);
					ctrl_transfer_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 6, stmnt_StartIndex); }

		}
		dbg.location(27, 23);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "stmnt"



	// $ANTLR start "decl_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:29:1: decl_stmnt : ( variable_decl_stmnt | class_decl_stmnt | enum_decl_stmnt | flags_decl_stmnt | function_decl_stmnt );
	public final void decl_stmnt() throws  {
		int decl_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "decl_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(29, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 7) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:29:12: ( variable_decl_stmnt | class_decl_stmnt | enum_decl_stmnt | flags_decl_stmnt | function_decl_stmnt )
			int alt7=5;
			try { dbg.enterDecision(7, decisionCanBacktrack[7]);

			switch ( input.LA(1) ) {
			case BOOL_TYPE_DENOTER:
			case FLOAT_TYPE_DENOTER:
			case INT_TYPE_DENOTER:
				{
				int LA7_1 = input.LA(2);
				if ( (LA7_1==IDENT) ) {
					int LA7_9 = input.LA(3);
					if ( (synpred10_XieXie()) ) {
						alt7=1;
					}
					else if ( (true) ) {
						alt7=5;
					}

				}

				else {
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 7, 1, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}

				}
				break;
			case IDENT:
				{
				int LA7_2 = input.LA(2);
				if ( (LA7_2==45) ) {
					int LA7_10 = input.LA(3);
					if ( (synpred10_XieXie()) ) {
						alt7=1;
					}
					else if ( (true) ) {
						alt7=5;
					}

				}
				else if ( (LA7_2==IDENT) ) {
					int LA7_11 = input.LA(3);
					if ( (synpred10_XieXie()) ) {
						alt7=1;
					}
					else if ( (true) ) {
						alt7=5;
					}

				}

				else {
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 7, 2, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}

				}
				break;
			case 68:
			case 74:
				{
				alt7=2;
				}
				break;
			case 73:
				{
				alt7=3;
				}
				break;
			case 75:
				{
				alt7=4;
				}
				break;
			case 61:
			case 83:
				{
				alt7=5;
				}
				break;
			default:
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 7, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}
			} finally {dbg.exitDecision(7);}

			switch (alt7) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:29:14: variable_decl_stmnt
					{
					dbg.location(29,14);
					pushFollow(FOLLOW_variable_decl_stmnt_in_decl_stmnt126);
					variable_decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:30:6: class_decl_stmnt
					{
					dbg.location(30,6);
					pushFollow(FOLLOW_class_decl_stmnt_in_decl_stmnt133);
					class_decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 3 :
					dbg.enterAlt(3);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:31:6: enum_decl_stmnt
					{
					dbg.location(31,6);
					pushFollow(FOLLOW_enum_decl_stmnt_in_decl_stmnt140);
					enum_decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 4 :
					dbg.enterAlt(4);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:32:6: flags_decl_stmnt
					{
					dbg.location(32,6);
					pushFollow(FOLLOW_flags_decl_stmnt_in_decl_stmnt147);
					flags_decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 5 :
					dbg.enterAlt(5);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:33:6: function_decl_stmnt
					{
					dbg.location(33,6);
					pushFollow(FOLLOW_function_decl_stmnt_in_decl_stmnt154);
					function_decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 7, decl_stmnt_StartIndex); }

		}
		dbg.location(33, 24);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "decl_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "decl_stmnt"



	// $ANTLR start "branch_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:35:1: branch_stmnt : ( if_stmnt | switch_stmnt );
	public final void branch_stmnt() throws  {
		int branch_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "branch_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(35, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 8) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:35:14: ( if_stmnt | switch_stmnt )
			int alt8=2;
			try { dbg.enterDecision(8, decisionCanBacktrack[8]);

			int LA8_0 = input.LA(1);
			if ( (LA8_0==79) ) {
				alt8=1;
			}
			else if ( (LA8_0==84) ) {
				alt8=2;
			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 8, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(8);}

			switch (alt8) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:35:16: if_stmnt
					{
					dbg.location(35,16);
					pushFollow(FOLLOW_if_stmnt_in_branch_stmnt162);
					if_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:36:7: switch_stmnt
					{
					dbg.location(36,7);
					pushFollow(FOLLOW_switch_stmnt_in_branch_stmnt170);
					switch_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 8, branch_stmnt_StartIndex); }

		}
		dbg.location(36, 18);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "branch_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "branch_stmnt"



	// $ANTLR start "loop_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:38:1: loop_stmnt : ( for_stmnt | for_each_stmnt | for_ever_stmnt | for_range_stmnt | while_stmnt | do_while_stmnt );
	public final void loop_stmnt() throws  {
		int loop_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "loop_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(38, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 9) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:38:12: ( for_stmnt | for_each_stmnt | for_ever_stmnt | for_range_stmnt | while_stmnt | do_while_stmnt )
			int alt9=6;
			try { dbg.enterDecision(9, decisionCanBacktrack[9]);

			switch ( input.LA(1) ) {
			case 76:
				{
				int LA9_1 = input.LA(2);
				if ( (LA9_1==BOOL_TYPE_DENOTER||LA9_1==FLOAT_TYPE_DENOTER||LA9_1==INT_TYPE_DENOTER||LA9_1==51) ) {
					alt9=1;
				}
				else if ( (LA9_1==IDENT) ) {
					int LA9_7 = input.LA(3);
					if ( (synpred15_XieXie()) ) {
						alt9=1;
					}
					else if ( (synpred18_XieXie()) ) {
						alt9=4;
					}

					else {
						if (state.backtracking>0) {state.failed=true; return;}
						int nvaeMark = input.mark();
						try {
							for (int nvaeConsume = 0; nvaeConsume < 3 - 1; nvaeConsume++) {
								input.consume();
							}
							NoViableAltException nvae =
								new NoViableAltException("", 9, 7, input);
							dbg.recognitionException(nvae);
							throw nvae;
						} finally {
							input.rewind(nvaeMark);
						}
					}

				}

				else {
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 9, 1, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}

				}
				break;
			case 77:
				{
				alt9=2;
				}
				break;
			case 78:
				{
				alt9=3;
				}
				break;
			case 85:
				{
				alt9=5;
				}
				break;
			case 71:
				{
				alt9=6;
				}
				break;
			default:
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 9, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}
			} finally {dbg.exitDecision(9);}

			switch (alt9) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:38:14: for_stmnt
					{
					dbg.location(38,14);
					pushFollow(FOLLOW_for_stmnt_in_loop_stmnt178);
					for_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:39:6: for_each_stmnt
					{
					dbg.location(39,6);
					pushFollow(FOLLOW_for_each_stmnt_in_loop_stmnt185);
					for_each_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 3 :
					dbg.enterAlt(3);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:40:6: for_ever_stmnt
					{
					dbg.location(40,6);
					pushFollow(FOLLOW_for_ever_stmnt_in_loop_stmnt192);
					for_ever_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 4 :
					dbg.enterAlt(4);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:41:6: for_range_stmnt
					{
					dbg.location(41,6);
					pushFollow(FOLLOW_for_range_stmnt_in_loop_stmnt199);
					for_range_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 5 :
					dbg.enterAlt(5);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:42:6: while_stmnt
					{
					dbg.location(42,6);
					pushFollow(FOLLOW_while_stmnt_in_loop_stmnt206);
					while_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 6 :
					dbg.enterAlt(6);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:43:6: do_while_stmnt
					{
					dbg.location(43,6);
					pushFollow(FOLLOW_do_while_stmnt_in_loop_stmnt213);
					do_while_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 9, loop_stmnt_StartIndex); }

		}
		dbg.location(43, 19);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "loop_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "loop_stmnt"



	// $ANTLR start "ctrl_transfer_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:45:1: ctrl_transfer_stmnt : ( break_stmnt | continue_stmnt | return_stmnt );
	public final void ctrl_transfer_stmnt() throws  {
		int ctrl_transfer_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "ctrl_transfer_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(45, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 10) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:45:21: ( break_stmnt | continue_stmnt | return_stmnt )
			int alt10=3;
			try { dbg.enterDecision(10, decisionCanBacktrack[10]);

			switch ( input.LA(1) ) {
			case 66:
				{
				alt10=1;
				}
				break;
			case 69:
				{
				alt10=2;
				}
				break;
			case 82:
				{
				alt10=3;
				}
				break;
			default:
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 10, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}
			} finally {dbg.exitDecision(10);}

			switch (alt10) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:45:23: break_stmnt
					{
					dbg.location(45,23);
					pushFollow(FOLLOW_break_stmnt_in_ctrl_transfer_stmnt221);
					break_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:46:8: continue_stmnt
					{
					dbg.location(46,8);
					pushFollow(FOLLOW_continue_stmnt_in_ctrl_transfer_stmnt230);
					continue_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 3 :
					dbg.enterAlt(3);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:47:8: return_stmnt
					{
					dbg.location(47,8);
					pushFollow(FOLLOW_return_stmnt_in_ctrl_transfer_stmnt239);
					return_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 10, ctrl_transfer_stmnt_StartIndex); }

		}
		dbg.location(47, 19);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "ctrl_transfer_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "ctrl_transfer_stmnt"



	// $ANTLR start "assign_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:49:1: assign_stmnt : ( copy_assign_stmnt | modify_assign_stmnt | post_operator_stmnt );
	public final void assign_stmnt() throws  {
		int assign_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "assign_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(49, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 11) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:49:14: ( copy_assign_stmnt | modify_assign_stmnt | post_operator_stmnt )
			int alt11=3;
			try { dbg.enterDecision(11, decisionCanBacktrack[11]);

			int LA11_0 = input.LA(1);
			if ( (LA11_0==IDENT) ) {
				switch ( input.LA(2) ) {
				case 45:
					{
					int LA11_2 = input.LA(3);
					if ( (synpred22_XieXie()) ) {
						alt11=1;
					}
					else if ( (synpred23_XieXie()) ) {
						alt11=2;
					}
					else if ( (true) ) {
						alt11=3;
					}

					}
					break;
				case 50:
					{
					alt11=1;
					}
					break;
				case 30:
				case 32:
				case 36:
				case 39:
				case 43:
				case 48:
				case 54:
				case 60:
				case 64:
				case 88:
					{
					alt11=2;
					}
					break;
				case 38:
				case 42:
					{
					alt11=3;
					}
					break;
				default:
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 11, 1, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}
			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 11, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(11);}

			switch (alt11) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:49:16: copy_assign_stmnt
					{
					dbg.location(49,16);
					pushFollow(FOLLOW_copy_assign_stmnt_in_assign_stmnt247);
					copy_assign_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:50:7: modify_assign_stmnt
					{
					dbg.location(50,7);
					pushFollow(FOLLOW_modify_assign_stmnt_in_assign_stmnt255);
					modify_assign_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 3 :
					dbg.enterAlt(3);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:51:7: post_operator_stmnt
					{
					dbg.location(51,7);
					pushFollow(FOLLOW_post_operator_stmnt_in_assign_stmnt263);
					post_operator_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 11, assign_stmnt_StartIndex); }

		}
		dbg.location(51, 25);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "assign_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "assign_stmnt"



	// $ANTLR start "copy_assign_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:54:1: copy_assign_stmnt : variable_ident ':=' expr ;
	public final void copy_assign_stmnt() throws  {
		int copy_assign_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "copy_assign_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(54, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 12) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:54:18: ( variable_ident ':=' expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:54:21: variable_ident ':=' expr
			{
			dbg.location(54,21);
			pushFollow(FOLLOW_variable_ident_in_copy_assign_stmnt272);
			variable_ident();
			state._fsp--;
			if (state.failed) return;dbg.location(54,36);
			match(input,50,FOLLOW_50_in_copy_assign_stmnt274); if (state.failed) return;dbg.location(54,41);
			pushFollow(FOLLOW_expr_in_copy_assign_stmnt276);
			expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 12, copy_assign_stmnt_StartIndex); }

		}
		dbg.location(54, 44);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "copy_assign_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "copy_assign_stmnt"



	// $ANTLR start "modify_assign_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:55:1: modify_assign_stmnt : variable_ident ( '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '|=' | '&=' | '^=' ) expr ;
	public final void modify_assign_stmnt() throws  {
		int modify_assign_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "modify_assign_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(55, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 13) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:55:20: ( variable_ident ( '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '|=' | '&=' | '^=' ) expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:55:22: variable_ident ( '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '|=' | '&=' | '^=' ) expr
			{
			dbg.location(55,22);
			pushFollow(FOLLOW_variable_ident_in_modify_assign_stmnt282);
			variable_ident();
			state._fsp--;
			if (state.failed) return;dbg.location(55,37);
			if ( input.LA(1)==30||input.LA(1)==32||input.LA(1)==36||input.LA(1)==39||input.LA(1)==43||input.LA(1)==48||input.LA(1)==54||input.LA(1)==60||input.LA(1)==64||input.LA(1)==88 ) {
				input.consume();
				state.errorRecovery=false;
				state.failed=false;
			}
			else {
				if (state.backtracking>0) {state.failed=true; return;}
				MismatchedSetException mse = new MismatchedSetException(null,input);
				dbg.recognitionException(mse);
				throw mse;
			}dbg.location(55,109);
			pushFollow(FOLLOW_expr_in_modify_assign_stmnt324);
			expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 13, modify_assign_stmnt_StartIndex); }

		}
		dbg.location(55, 112);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "modify_assign_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "modify_assign_stmnt"



	// $ANTLR start "post_operator_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:56:1: post_operator_stmnt : variable_ident ( '++' | '--' ) ;
	public final void post_operator_stmnt() throws  {
		int post_operator_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "post_operator_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(56, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 14) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:56:20: ( variable_ident ( '++' | '--' ) )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:56:22: variable_ident ( '++' | '--' )
			{
			dbg.location(56,22);
			pushFollow(FOLLOW_variable_ident_in_post_operator_stmnt330);
			variable_ident();
			state._fsp--;
			if (state.failed) return;dbg.location(56,37);
			if ( input.LA(1)==38||input.LA(1)==42 ) {
				input.consume();
				state.errorRecovery=false;
				state.failed=false;
			}
			else {
				if (state.backtracking>0) {state.failed=true; return;}
				MismatchedSetException mse = new MismatchedSetException(null,input);
				dbg.recognitionException(mse);
				throw mse;
			}
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 14, post_operator_stmnt_StartIndex); }

		}
		dbg.location(56, 49);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "post_operator_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "post_operator_stmnt"



	// $ANTLR start "if_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:59:1: if_stmnt : 'if' if_condition code_block ( else_stmnt )? ;
	public final void if_stmnt() throws  {
		int if_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "if_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(59, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 15) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:59:9: ( 'if' if_condition code_block ( else_stmnt )? )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:59:12: 'if' if_condition code_block ( else_stmnt )?
			{
			dbg.location(59,12);
			match(input,79,FOLLOW_79_in_if_stmnt347); if (state.failed) return;dbg.location(59,17);
			pushFollow(FOLLOW_if_condition_in_if_stmnt349);
			if_condition();
			state._fsp--;
			if (state.failed) return;dbg.location(59,30);
			pushFollow(FOLLOW_code_block_in_if_stmnt351);
			code_block();
			state._fsp--;
			if (state.failed) return;dbg.location(59,41);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:59:41: ( else_stmnt )?
			int alt12=2;
			try { dbg.enterSubRule(12);
			try { dbg.enterDecision(12, decisionCanBacktrack[12]);

			int LA12_0 = input.LA(1);
			if ( (LA12_0==72) ) {
				alt12=1;
			}
			} finally {dbg.exitDecision(12);}

			switch (alt12) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:59:41: else_stmnt
					{
					dbg.location(59,41);
					pushFollow(FOLLOW_else_stmnt_in_if_stmnt353);
					else_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(12);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 15, if_stmnt_StartIndex); }

		}
		dbg.location(59, 51);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "if_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "if_stmnt"



	// $ANTLR start "if_condition"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:60:1: if_condition : expr ;
	public final void if_condition() throws  {
		int if_condition_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "if_condition");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(60, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 16) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:60:13: ( expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:60:15: expr
			{
			dbg.location(60,15);
			pushFollow(FOLLOW_expr_in_if_condition360);
			expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 16, if_condition_StartIndex); }

		}
		dbg.location(60, 18);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "if_condition");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "if_condition"



	// $ANTLR start "else_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:61:1: else_stmnt : 'else' ( if_stmnt | code_block ) ;
	public final void else_stmnt() throws  {
		int else_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "else_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(61, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 17) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:61:11: ( 'else' ( if_stmnt | code_block ) )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:61:14: 'else' ( if_stmnt | code_block )
			{
			dbg.location(61,14);
			match(input,72,FOLLOW_72_in_else_stmnt367); if (state.failed) return;dbg.location(61,21);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:61:21: ( if_stmnt | code_block )
			int alt13=2;
			try { dbg.enterSubRule(13);
			try { dbg.enterDecision(13, decisionCanBacktrack[13]);

			int LA13_0 = input.LA(1);
			if ( (LA13_0==79) ) {
				alt13=1;
			}
			else if ( (LA13_0==86) ) {
				alt13=2;
			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 13, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(13);}

			switch (alt13) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:61:22: if_stmnt
					{
					dbg.location(61,22);
					pushFollow(FOLLOW_if_stmnt_in_else_stmnt370);
					if_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:61:33: code_block
					{
					dbg.location(61,33);
					pushFollow(FOLLOW_code_block_in_else_stmnt374);
					code_block();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(13);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 17, else_stmnt_StartIndex); }

		}
		dbg.location(61, 43);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "else_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "else_stmnt"



	// $ANTLR start "switch_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:63:1: switch_stmnt : 'switch' expr '{' ( switch_case_list )? '}' ;
	public final void switch_stmnt() throws  {
		int switch_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "switch_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(63, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 18) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:63:13: ( 'switch' expr '{' ( switch_case_list )? '}' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:63:16: 'switch' expr '{' ( switch_case_list )? '}'
			{
			dbg.location(63,16);
			match(input,84,FOLLOW_84_in_switch_stmnt383); if (state.failed) return;dbg.location(63,25);
			pushFollow(FOLLOW_expr_in_switch_stmnt385);
			expr();
			state._fsp--;
			if (state.failed) return;dbg.location(63,30);
			match(input,86,FOLLOW_86_in_switch_stmnt387); if (state.failed) return;dbg.location(63,34);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:63:34: ( switch_case_list )?
			int alt14=2;
			try { dbg.enterSubRule(14);
			try { dbg.enterDecision(14, decisionCanBacktrack[14]);

			int LA14_0 = input.LA(1);
			if ( (LA14_0==67||LA14_0==70) ) {
				alt14=1;
			}
			} finally {dbg.exitDecision(14);}

			switch (alt14) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:63:34: switch_case_list
					{
					dbg.location(63,34);
					pushFollow(FOLLOW_switch_case_list_in_switch_stmnt389);
					switch_case_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(14);}
			dbg.location(63,52);
			match(input,89,FOLLOW_89_in_switch_stmnt392); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 18, switch_stmnt_StartIndex); }

		}
		dbg.location(63, 54);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "switch_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "switch_stmnt"



	// $ANTLR start "switch_case_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:64:1: switch_case_list : ( switch_case )+ ;
	public final void switch_case_list() throws  {
		int switch_case_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "switch_case_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(64, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 19) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:64:17: ( ( switch_case )+ )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:64:19: ( switch_case )+
			{
			dbg.location(64,19);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:64:19: ( switch_case )+
			int cnt15=0;
			try { dbg.enterSubRule(15);

			loop15:
			while (true) {
				int alt15=2;
				try { dbg.enterDecision(15, decisionCanBacktrack[15]);

				int LA15_0 = input.LA(1);
				if ( (LA15_0==67||LA15_0==70) ) {
					alt15=1;
				}

				} finally {dbg.exitDecision(15);}

				switch (alt15) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:64:19: switch_case
					{
					dbg.location(64,19);
					pushFollow(FOLLOW_switch_case_in_switch_case_list398);
					switch_case();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					if ( cnt15 >= 1 ) break loop15;
					if (state.backtracking>0) {state.failed=true; return;}
					EarlyExitException eee = new EarlyExitException(15, input);
					dbg.recognitionException(eee);

					throw eee;
				}
				cnt15++;
			}
			} finally {dbg.exitSubRule(15);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 19, switch_case_list_StartIndex); }

		}
		dbg.location(64, 30);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "switch_case_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "switch_case_list"



	// $ANTLR start "switch_case"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:65:1: switch_case : ( case_label | default_label ) stmnt_list ;
	public final void switch_case() throws  {
		int switch_case_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "switch_case");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(65, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 20) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:65:12: ( ( case_label | default_label ) stmnt_list )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:65:15: ( case_label | default_label ) stmnt_list
			{
			dbg.location(65,15);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:65:15: ( case_label | default_label )
			int alt16=2;
			try { dbg.enterSubRule(16);
			try { dbg.enterDecision(16, decisionCanBacktrack[16]);

			int LA16_0 = input.LA(1);
			if ( (LA16_0==67) ) {
				alt16=1;
			}
			else if ( (LA16_0==70) ) {
				alt16=2;
			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 16, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(16);}

			switch (alt16) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:65:16: case_label
					{
					dbg.location(65,16);
					pushFollow(FOLLOW_case_label_in_switch_case407);
					case_label();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:65:29: default_label
					{
					dbg.location(65,29);
					pushFollow(FOLLOW_default_label_in_switch_case411);
					default_label();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(16);}
			dbg.location(65,44);
			pushFollow(FOLLOW_stmnt_list_in_switch_case414);
			stmnt_list();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 20, switch_case_StartIndex); }

		}
		dbg.location(65, 53);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "switch_case");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "switch_case"



	// $ANTLR start "case_label"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:66:1: case_label : 'case' case_item_list ':' ;
	public final void case_label() throws  {
		int case_label_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "case_label");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(66, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 21) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:66:11: ( 'case' case_item_list ':' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:66:15: 'case' case_item_list ':'
			{
			dbg.location(66,15);
			match(input,67,FOLLOW_67_in_case_label422); if (state.failed) return;dbg.location(66,22);
			pushFollow(FOLLOW_case_item_list_in_case_label424);
			case_item_list();
			state._fsp--;
			if (state.failed) return;dbg.location(66,37);
			match(input,49,FOLLOW_49_in_case_label426); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 21, case_label_StartIndex); }

		}
		dbg.location(66, 39);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "case_label");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "case_label"



	// $ANTLR start "default_label"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:67:1: default_label : 'default' ':' ;
	public final void default_label() throws  {
		int default_label_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "default_label");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(67, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 22) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:67:14: ( 'default' ':' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:67:17: 'default' ':'
			{
			dbg.location(67,17);
			match(input,70,FOLLOW_70_in_default_label433); if (state.failed) return;dbg.location(67,27);
			match(input,49,FOLLOW_49_in_default_label435); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 22, default_label_StartIndex); }

		}
		dbg.location(67, 29);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "default_label");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "default_label"



	// $ANTLR start "case_item_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:68:1: case_item_list : case_item ( ',' case_item )* ;
	public final void case_item_list() throws  {
		int case_item_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "case_item_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(68, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 23) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:68:15: ( case_item ( ',' case_item )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:68:18: case_item ( ',' case_item )*
			{
			dbg.location(68,18);
			pushFollow(FOLLOW_case_item_in_case_item_list442);
			case_item();
			state._fsp--;
			if (state.failed) return;dbg.location(68,28);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:68:28: ( ',' case_item )*
			try { dbg.enterSubRule(17);

			loop17:
			while (true) {
				int alt17=2;
				try { dbg.enterDecision(17, decisionCanBacktrack[17]);

				int LA17_0 = input.LA(1);
				if ( (LA17_0==40) ) {
					alt17=1;
				}

				} finally {dbg.exitDecision(17);}

				switch (alt17) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:68:29: ',' case_item
					{
					dbg.location(68,29);
					match(input,40,FOLLOW_40_in_case_item_list445); if (state.failed) return;dbg.location(68,33);
					pushFollow(FOLLOW_case_item_in_case_item_list447);
					case_item();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop17;
				}
			}
			} finally {dbg.exitSubRule(17);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 23, case_item_list_StartIndex); }

		}
		dbg.location(68, 43);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "case_item_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "case_item_list"



	// $ANTLR start "case_item"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:69:1: case_item : pattern ;
	public final void case_item() throws  {
		int case_item_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "case_item");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(69, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 24) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:69:10: ( pattern )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:69:14: pattern
			{
			dbg.location(69,14);
			pushFollow(FOLLOW_pattern_in_case_item457);
			pattern();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 24, case_item_StartIndex); }

		}
		dbg.location(69, 20);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "case_item");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "case_item"



	// $ANTLR start "break_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:72:1: break_stmnt : 'break' ;
	public final void break_stmnt() throws  {
		int break_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "break_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(72, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 25) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:72:12: ( 'break' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:72:14: 'break'
			{
			dbg.location(72,14);
			match(input,66,FOLLOW_66_in_break_stmnt465); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 25, break_stmnt_StartIndex); }

		}
		dbg.location(72, 20);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "break_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "break_stmnt"



	// $ANTLR start "continue_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:73:1: continue_stmnt : 'continue' ;
	public final void continue_stmnt() throws  {
		int continue_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "continue_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(73, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 26) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:73:15: ( 'continue' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:73:17: 'continue'
			{
			dbg.location(73,17);
			match(input,69,FOLLOW_69_in_continue_stmnt471); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 26, continue_stmnt_StartIndex); }

		}
		dbg.location(73, 26);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "continue_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "continue_stmnt"



	// $ANTLR start "return_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:74:1: return_stmnt : 'return' ( expr )? ;
	public final void return_stmnt() throws  {
		int return_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "return_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(74, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 27) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:74:13: ( 'return' ( expr )? )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:74:15: 'return' ( expr )?
			{
			dbg.location(74,15);
			match(input,82,FOLLOW_82_in_return_stmnt477); if (state.failed) return;dbg.location(74,24);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:74:24: ( expr )?
			int alt18=2;
			try { dbg.enterSubRule(18);
			try { dbg.enterDecision(18, decisionCanBacktrack[18]);

			int LA18_0 = input.LA(1);
			if ( (LA18_0==33||LA18_0==41||LA18_0==80||LA18_0==86||LA18_0==90) ) {
				alt18=1;
			}
			else if ( (LA18_0==IDENT) ) {
				int LA18_2 = input.LA(2);
				if ( (LA18_2==45) ) {
					int LA18_26 = input.LA(3);
					if ( (synpred40_XieXie()) ) {
						alt18=1;
					}
				}
				else if ( (LA18_2==33) ) {
					alt18=1;
				}
			}
			} finally {dbg.exitDecision(18);}

			switch (alt18) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:74:24: expr
					{
					dbg.location(74,24);
					pushFollow(FOLLOW_expr_in_return_stmnt479);
					expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(18);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 27, return_stmnt_StartIndex); }

		}
		dbg.location(74, 28);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "return_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "return_stmnt"



	// $ANTLR start "pattern"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:77:1: pattern : ( expr_pattern | enum_case_pattern );
	public final void pattern() throws  {
		int pattern_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "pattern");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(77, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 28) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:77:9: ( expr_pattern | enum_case_pattern )
			int alt19=2;
			try { dbg.enterDecision(19, decisionCanBacktrack[19]);

			int LA19_0 = input.LA(1);
			if ( (LA19_0==33||LA19_0==41||LA19_0==80||LA19_0==86||LA19_0==90) ) {
				alt19=1;
			}
			else if ( (LA19_0==IDENT) ) {
				switch ( input.LA(2) ) {
				case 45:
					{
					int LA19_5 = input.LA(3);
					if ( (synpred41_XieXie()) ) {
						alt19=1;
					}
					else if ( (true) ) {
						alt19=2;
					}

					}
					break;
				case 33:
					{
					alt19=1;
					}
					break;
				case EOF:
				case 40:
				case 49:
					{
					alt19=2;
					}
					break;
				default:
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 19, 2, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}
			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 19, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(19);}

			switch (alt19) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:77:11: expr_pattern
					{
					dbg.location(77,11);
					pushFollow(FOLLOW_expr_pattern_in_pattern489);
					expr_pattern();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:78:5: enum_case_pattern
					{
					dbg.location(78,5);
					pushFollow(FOLLOW_enum_case_pattern_in_pattern495);
					enum_case_pattern();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 28, pattern_StartIndex); }

		}
		dbg.location(78, 21);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "pattern");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "pattern"



	// $ANTLR start "expr_pattern"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:80:1: expr_pattern : expr ;
	public final void expr_pattern() throws  {
		int expr_pattern_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "expr_pattern");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(80, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 29) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:80:13: ( expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:80:17: expr
			{
			dbg.location(80,17);
			pushFollow(FOLLOW_expr_in_expr_pattern504);
			expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 29, expr_pattern_StartIndex); }

		}
		dbg.location(80, 20);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "expr_pattern");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "expr_pattern"



	// $ANTLR start "enum_case_pattern"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:82:1: enum_case_pattern : ( variable_ident '.' )? enum_case_name ;
	public final void enum_case_pattern() throws  {
		int enum_case_pattern_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "enum_case_pattern");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(82, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 30) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:82:18: ( ( variable_ident '.' )? enum_case_name )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:82:20: ( variable_ident '.' )? enum_case_name
			{
			dbg.location(82,20);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:82:20: ( variable_ident '.' )?
			int alt20=2;
			try { dbg.enterSubRule(20);
			try { dbg.enterDecision(20, decisionCanBacktrack[20]);

			int LA20_0 = input.LA(1);
			if ( (LA20_0==IDENT) ) {
				int LA20_1 = input.LA(2);
				if ( (LA20_1==45) ) {
					alt20=1;
				}
			}
			} finally {dbg.exitDecision(20);}

			switch (alt20) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:82:21: variable_ident '.'
					{
					dbg.location(82,21);
					pushFollow(FOLLOW_variable_ident_in_enum_case_pattern512);
					variable_ident();
					state._fsp--;
					if (state.failed) return;dbg.location(82,36);
					match(input,45,FOLLOW_45_in_enum_case_pattern514); if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(20);}
			dbg.location(82,42);
			pushFollow(FOLLOW_enum_case_name_in_enum_case_pattern518);
			enum_case_name();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 30, enum_case_pattern_StartIndex); }

		}
		dbg.location(82, 55);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "enum_case_pattern");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "enum_case_pattern"



	// $ANTLR start "enum_case_name"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:83:1: enum_case_name : IDENT ;
	public final void enum_case_name() throws  {
		int enum_case_name_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "enum_case_name");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(83, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 31) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:83:15: ( IDENT )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:83:18: IDENT
			{
			dbg.location(83,18);
			match(input,IDENT,FOLLOW_IDENT_in_enum_case_name525); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 31, enum_case_name_StartIndex); }

		}
		dbg.location(83, 22);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "enum_case_name");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "enum_case_name"



	// $ANTLR start "for_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:1: for_stmnt : 'for' ( for_init )? ';' ( expr )? ';' ( assign_stmnt )? code_block ;
	public final void for_stmnt() throws  {
		int for_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "for_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(86, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 32) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:10: ( 'for' ( for_init )? ';' ( expr )? ';' ( assign_stmnt )? code_block )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:14: 'for' ( for_init )? ';' ( expr )? ';' ( assign_stmnt )? code_block
			{
			dbg.location(86,14);
			match(input,76,FOLLOW_76_in_for_stmnt535); if (state.failed) return;dbg.location(86,20);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:20: ( for_init )?
			int alt21=2;
			try { dbg.enterSubRule(21);
			try { dbg.enterDecision(21, decisionCanBacktrack[21]);

			int LA21_0 = input.LA(1);
			if ( (LA21_0==BOOL_TYPE_DENOTER||LA21_0==FLOAT_TYPE_DENOTER||LA21_0==IDENT||LA21_0==INT_TYPE_DENOTER) ) {
				alt21=1;
			}
			} finally {dbg.exitDecision(21);}

			switch (alt21) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:20: for_init
					{
					dbg.location(86,20);
					pushFollow(FOLLOW_for_init_in_for_stmnt537);
					for_init();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(21);}
			dbg.location(86,30);
			match(input,51,FOLLOW_51_in_for_stmnt540); if (state.failed) return;dbg.location(86,34);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:34: ( expr )?
			int alt22=2;
			try { dbg.enterSubRule(22);
			try { dbg.enterDecision(22, decisionCanBacktrack[22]);

			int LA22_0 = input.LA(1);
			if ( (LA22_0==IDENT||LA22_0==33||LA22_0==41||LA22_0==80||LA22_0==86||LA22_0==90) ) {
				alt22=1;
			}
			} finally {dbg.exitDecision(22);}

			switch (alt22) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:34: expr
					{
					dbg.location(86,34);
					pushFollow(FOLLOW_expr_in_for_stmnt542);
					expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(22);}
			dbg.location(86,40);
			match(input,51,FOLLOW_51_in_for_stmnt545); if (state.failed) return;dbg.location(86,44);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:44: ( assign_stmnt )?
			int alt23=2;
			try { dbg.enterSubRule(23);
			try { dbg.enterDecision(23, decisionCanBacktrack[23]);

			int LA23_0 = input.LA(1);
			if ( (LA23_0==IDENT) ) {
				alt23=1;
			}
			} finally {dbg.exitDecision(23);}

			switch (alt23) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:86:44: assign_stmnt
					{
					dbg.location(86,44);
					pushFollow(FOLLOW_assign_stmnt_in_for_stmnt547);
					assign_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(23);}
			dbg.location(86,58);
			pushFollow(FOLLOW_code_block_in_for_stmnt550);
			code_block();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 32, for_stmnt_StartIndex); }

		}
		dbg.location(86, 67);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "for_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "for_stmnt"



	// $ANTLR start "for_each_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:87:1: for_each_stmnt : 'foreach' for_each_init ':' expr code_block ;
	public final void for_each_stmnt() throws  {
		int for_each_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "for_each_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(87, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 33) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:87:15: ( 'foreach' for_each_init ':' expr code_block )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:87:18: 'foreach' for_each_init ':' expr code_block
			{
			dbg.location(87,18);
			match(input,77,FOLLOW_77_in_for_each_stmnt557); if (state.failed) return;dbg.location(87,28);
			pushFollow(FOLLOW_for_each_init_in_for_each_stmnt559);
			for_each_init();
			state._fsp--;
			if (state.failed) return;dbg.location(87,42);
			match(input,49,FOLLOW_49_in_for_each_stmnt561); if (state.failed) return;dbg.location(87,46);
			pushFollow(FOLLOW_expr_in_for_each_stmnt563);
			expr();
			state._fsp--;
			if (state.failed) return;dbg.location(87,51);
			pushFollow(FOLLOW_code_block_in_for_each_stmnt565);
			code_block();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 33, for_each_stmnt_StartIndex); }

		}
		dbg.location(87, 60);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "for_each_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "for_each_stmnt"



	// $ANTLR start "for_ever_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:88:1: for_ever_stmnt : 'forever' code_block ;
	public final void for_ever_stmnt() throws  {
		int for_ever_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "for_ever_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(88, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 34) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:88:15: ( 'forever' code_block )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:88:18: 'forever' code_block
			{
			dbg.location(88,18);
			match(input,78,FOLLOW_78_in_for_ever_stmnt572); if (state.failed) return;dbg.location(88,28);
			pushFollow(FOLLOW_code_block_in_for_ever_stmnt574);
			code_block();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 34, for_ever_stmnt_StartIndex); }

		}
		dbg.location(88, 37);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "for_ever_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "for_ever_stmnt"



	// $ANTLR start "for_range_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:89:1: for_range_stmnt : 'for' variable_name ':' for_range '..' for_range ( '->' for_range )? code_block ;
	public final void for_range_stmnt() throws  {
		int for_range_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "for_range_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(89, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 35) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:89:16: ( 'for' variable_name ':' for_range '..' for_range ( '->' for_range )? code_block )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:89:18: 'for' variable_name ':' for_range '..' for_range ( '->' for_range )? code_block
			{
			dbg.location(89,18);
			match(input,76,FOLLOW_76_in_for_range_stmnt580); if (state.failed) return;dbg.location(89,24);
			pushFollow(FOLLOW_variable_name_in_for_range_stmnt582);
			variable_name();
			state._fsp--;
			if (state.failed) return;dbg.location(89,38);
			match(input,49,FOLLOW_49_in_for_range_stmnt584); if (state.failed) return;dbg.location(89,42);
			pushFollow(FOLLOW_for_range_in_for_range_stmnt586);
			for_range();
			state._fsp--;
			if (state.failed) return;dbg.location(89,52);
			match(input,46,FOLLOW_46_in_for_range_stmnt588); if (state.failed) return;dbg.location(89,57);
			pushFollow(FOLLOW_for_range_in_for_range_stmnt590);
			for_range();
			state._fsp--;
			if (state.failed) return;dbg.location(89,67);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:89:67: ( '->' for_range )?
			int alt24=2;
			try { dbg.enterSubRule(24);
			try { dbg.enterDecision(24, decisionCanBacktrack[24]);

			int LA24_0 = input.LA(1);
			if ( (LA24_0==44) ) {
				alt24=1;
			}
			} finally {dbg.exitDecision(24);}

			switch (alt24) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:89:68: '->' for_range
					{
					dbg.location(89,68);
					match(input,44,FOLLOW_44_in_for_range_stmnt593); if (state.failed) return;dbg.location(89,73);
					pushFollow(FOLLOW_for_range_in_for_range_stmnt595);
					for_range();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(24);}
			dbg.location(89,85);
			pushFollow(FOLLOW_code_block_in_for_range_stmnt599);
			code_block();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 35, for_range_stmnt_StartIndex); }

		}
		dbg.location(89, 94);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "for_range_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "for_range_stmnt"



	// $ANTLR start "while_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:90:1: while_stmnt : 'while' expr code_block ;
	public final void while_stmnt() throws  {
		int while_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "while_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(90, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 36) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:90:12: ( 'while' expr code_block )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:90:15: 'while' expr code_block
			{
			dbg.location(90,15);
			match(input,85,FOLLOW_85_in_while_stmnt606); if (state.failed) return;dbg.location(90,23);
			pushFollow(FOLLOW_expr_in_while_stmnt608);
			expr();
			state._fsp--;
			if (state.failed) return;dbg.location(90,28);
			pushFollow(FOLLOW_code_block_in_while_stmnt610);
			code_block();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 36, while_stmnt_StartIndex); }

		}
		dbg.location(90, 37);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "while_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "while_stmnt"



	// $ANTLR start "do_while_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:91:1: do_while_stmnt : 'do' code_block 'while' expr ;
	public final void do_while_stmnt() throws  {
		int do_while_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "do_while_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(91, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 37) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:91:15: ( 'do' code_block 'while' expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:91:18: 'do' code_block 'while' expr
			{
			dbg.location(91,18);
			match(input,71,FOLLOW_71_in_do_while_stmnt617); if (state.failed) return;dbg.location(91,23);
			pushFollow(FOLLOW_code_block_in_do_while_stmnt619);
			code_block();
			state._fsp--;
			if (state.failed) return;dbg.location(91,34);
			match(input,85,FOLLOW_85_in_do_while_stmnt621); if (state.failed) return;dbg.location(91,42);
			pushFollow(FOLLOW_expr_in_do_while_stmnt623);
			expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 37, do_while_stmnt_StartIndex); }

		}
		dbg.location(91, 45);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "do_while_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "do_while_stmnt"



	// $ANTLR start "for_init"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:93:1: for_init : ( variable_decl_stmnt | assign_stmnt );
	public final void for_init() throws  {
		int for_init_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "for_init");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(93, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 38) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:93:9: ( variable_decl_stmnt | assign_stmnt )
			int alt25=2;
			try { dbg.enterDecision(25, decisionCanBacktrack[25]);

			int LA25_0 = input.LA(1);
			if ( (LA25_0==BOOL_TYPE_DENOTER||LA25_0==FLOAT_TYPE_DENOTER||LA25_0==INT_TYPE_DENOTER) ) {
				alt25=1;
			}
			else if ( (LA25_0==IDENT) ) {
				switch ( input.LA(2) ) {
				case 45:
					{
					int LA25_3 = input.LA(3);
					if ( (synpred47_XieXie()) ) {
						alt25=1;
					}
					else if ( (true) ) {
						alt25=2;
					}

					}
					break;
				case IDENT:
					{
					alt25=1;
					}
					break;
				case 30:
				case 32:
				case 36:
				case 38:
				case 39:
				case 42:
				case 43:
				case 48:
				case 50:
				case 54:
				case 60:
				case 64:
				case 88:
					{
					alt25=2;
					}
					break;
				default:
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 25, 2, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}
			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 25, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(25);}

			switch (alt25) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:93:12: variable_decl_stmnt
					{
					dbg.location(93,12);
					pushFollow(FOLLOW_variable_decl_stmnt_in_for_init631);
					variable_decl_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:93:34: assign_stmnt
					{
					dbg.location(93,34);
					pushFollow(FOLLOW_assign_stmnt_in_for_init635);
					assign_stmnt();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 38, for_init_StartIndex); }

		}
		dbg.location(93, 45);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "for_init");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "for_init"



	// $ANTLR start "for_each_init"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:94:1: for_each_init : variable_decl_stmnt ;
	public final void for_each_init() throws  {
		int for_each_init_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "for_each_init");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(94, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 39) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:94:14: ( variable_decl_stmnt )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:94:16: variable_decl_stmnt
			{
			dbg.location(94,16);
			pushFollow(FOLLOW_variable_decl_stmnt_in_for_each_init641);
			variable_decl_stmnt();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 39, for_each_init_StartIndex); }

		}
		dbg.location(94, 34);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "for_each_init");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "for_each_init"



	// $ANTLR start "for_range"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:95:1: for_range : INT_LITERAL ;
	public final void for_range() throws  {
		int for_range_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "for_range");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(95, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 40) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:95:10: ( INT_LITERAL )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:95:13: INT_LITERAL
			{
			dbg.location(95,13);
			match(input,INT_LITERAL,FOLLOW_INT_LITERAL_in_for_range648); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 40, for_range_StartIndex); }

		}
		dbg.location(95, 23);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "for_range");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "for_range"



	// $ANTLR start "attribute_prefix"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:98:1: attribute_prefix : '[[' attribute_list ']]' ;
	public final void attribute_prefix() throws  {
		int attribute_prefix_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "attribute_prefix");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(98, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 41) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:98:17: ( '[[' attribute_list ']]' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:98:21: '[[' attribute_list ']]'
			{
			dbg.location(98,21);
			match(input,61,FOLLOW_61_in_attribute_prefix658); if (state.failed) return;dbg.location(98,26);
			pushFollow(FOLLOW_attribute_list_in_attribute_prefix660);
			attribute_list();
			state._fsp--;
			if (state.failed) return;dbg.location(98,41);
			match(input,62,FOLLOW_62_in_attribute_prefix662); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 41, attribute_prefix_StartIndex); }

		}
		dbg.location(98, 44);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "attribute_prefix");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "attribute_prefix"



	// $ANTLR start "attribute_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:99:1: attribute_list : attribute ( ',' attribute )+ ;
	public final void attribute_list() throws  {
		int attribute_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "attribute_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(99, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 42) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:99:15: ( attribute ( ',' attribute )+ )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:99:20: attribute ( ',' attribute )+
			{
			dbg.location(99,20);
			pushFollow(FOLLOW_attribute_in_attribute_list671);
			attribute();
			state._fsp--;
			if (state.failed) return;dbg.location(99,30);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:99:30: ( ',' attribute )+
			int cnt26=0;
			try { dbg.enterSubRule(26);

			loop26:
			while (true) {
				int alt26=2;
				try { dbg.enterDecision(26, decisionCanBacktrack[26]);

				int LA26_0 = input.LA(1);
				if ( (LA26_0==40) ) {
					alt26=1;
				}

				} finally {dbg.exitDecision(26);}

				switch (alt26) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:99:31: ',' attribute
					{
					dbg.location(99,31);
					match(input,40,FOLLOW_40_in_attribute_list674); if (state.failed) return;dbg.location(99,35);
					pushFollow(FOLLOW_attribute_in_attribute_list676);
					attribute();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					if ( cnt26 >= 1 ) break loop26;
					if (state.backtracking>0) {state.failed=true; return;}
					EarlyExitException eee = new EarlyExitException(26, input);
					dbg.recognitionException(eee);

					throw eee;
				}
				cnt26++;
			}
			} finally {dbg.exitSubRule(26);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 42, attribute_list_StartIndex); }

		}
		dbg.location(99, 45);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "attribute_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "attribute_list"



	// $ANTLR start "attribute"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:100:1: attribute : IDENT ( '(' attribute_argument_list ')' )? ;
	public final void attribute() throws  {
		int attribute_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "attribute");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(100, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 43) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:100:10: ( IDENT ( '(' attribute_argument_list ')' )? )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:100:16: IDENT ( '(' attribute_argument_list ')' )?
			{
			dbg.location(100,16);
			match(input,IDENT,FOLLOW_IDENT_in_attribute688); if (state.failed) return;dbg.location(100,22);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:100:22: ( '(' attribute_argument_list ')' )?
			int alt27=2;
			try { dbg.enterSubRule(27);
			try { dbg.enterDecision(27, decisionCanBacktrack[27]);

			int LA27_0 = input.LA(1);
			if ( (LA27_0==33) ) {
				alt27=1;
			}
			} finally {dbg.exitDecision(27);}

			switch (alt27) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:100:23: '(' attribute_argument_list ')'
					{
					dbg.location(100,23);
					match(input,33,FOLLOW_33_in_attribute691); if (state.failed) return;dbg.location(100,27);
					pushFollow(FOLLOW_attribute_argument_list_in_attribute693);
					attribute_argument_list();
					state._fsp--;
					if (state.failed) return;dbg.location(100,51);
					match(input,34,FOLLOW_34_in_attribute695); if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(27);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 43, attribute_StartIndex); }

		}
		dbg.location(100, 55);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "attribute");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "attribute"



	// $ANTLR start "attribute_argument_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:101:1: attribute_argument_list : attribute_argument ( ',' attribute_argument )+ ;
	public final void attribute_argument_list() throws  {
		int attribute_argument_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "attribute_argument_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(101, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 44) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:101:24: ( attribute_argument ( ',' attribute_argument )+ )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:101:26: attribute_argument ( ',' attribute_argument )+
			{
			dbg.location(101,26);
			pushFollow(FOLLOW_attribute_argument_in_attribute_argument_list703);
			attribute_argument();
			state._fsp--;
			if (state.failed) return;dbg.location(101,45);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:101:45: ( ',' attribute_argument )+
			int cnt28=0;
			try { dbg.enterSubRule(28);

			loop28:
			while (true) {
				int alt28=2;
				try { dbg.enterDecision(28, decisionCanBacktrack[28]);

				int LA28_0 = input.LA(1);
				if ( (LA28_0==40) ) {
					alt28=1;
				}

				} finally {dbg.exitDecision(28);}

				switch (alt28) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:101:46: ',' attribute_argument
					{
					dbg.location(101,46);
					match(input,40,FOLLOW_40_in_attribute_argument_list706); if (state.failed) return;dbg.location(101,50);
					pushFollow(FOLLOW_attribute_argument_in_attribute_argument_list708);
					attribute_argument();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					if ( cnt28 >= 1 ) break loop28;
					if (state.backtracking>0) {state.failed=true; return;}
					EarlyExitException eee = new EarlyExitException(28, input);
					dbg.recognitionException(eee);

					throw eee;
				}
				cnt28++;
			}
			} finally {dbg.exitSubRule(28);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 44, attribute_argument_list_StartIndex); }

		}
		dbg.location(101, 69);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "attribute_argument_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "attribute_argument_list"



	// $ANTLR start "attribute_argument"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:102:1: attribute_argument : expr ;
	public final void attribute_argument() throws  {
		int attribute_argument_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "attribute_argument");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(102, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 45) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:102:19: ( expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:102:23: expr
			{
			dbg.location(102,23);
			pushFollow(FOLLOW_expr_in_attribute_argument718);
			expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 45, attribute_argument_StartIndex); }

		}
		dbg.location(102, 26);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "attribute_argument");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "attribute_argument"



	// $ANTLR start "variable_ident"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:105:1: variable_ident : variable_name ( '.' variable_name )* ;
	public final void variable_ident() throws  {
		int variable_ident_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "variable_ident");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(105, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 46) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:105:15: ( variable_name ( '.' variable_name )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:105:18: variable_name ( '.' variable_name )*
			{
			dbg.location(105,18);
			pushFollow(FOLLOW_variable_name_in_variable_ident727);
			variable_name();
			state._fsp--;
			if (state.failed) return;dbg.location(105,32);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:105:32: ( '.' variable_name )*
			try { dbg.enterSubRule(29);

			loop29:
			while (true) {
				int alt29=2;
				try { dbg.enterDecision(29, decisionCanBacktrack[29]);

				int LA29_0 = input.LA(1);
				if ( (LA29_0==45) ) {
					int LA29_4 = input.LA(2);
					if ( (LA29_4==IDENT) ) {
						int LA29_11 = input.LA(3);
						if ( (synpred51_XieXie()) ) {
							alt29=1;
						}

					}

				}

				} finally {dbg.exitDecision(29);}

				switch (alt29) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:105:33: '.' variable_name
					{
					dbg.location(105,33);
					match(input,45,FOLLOW_45_in_variable_ident730); if (state.failed) return;dbg.location(105,37);
					pushFollow(FOLLOW_variable_name_in_variable_ident732);
					variable_name();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop29;
				}
			}
			} finally {dbg.exitSubRule(29);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 46, variable_ident_StartIndex); }

		}
		dbg.location(105, 51);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "variable_ident");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "variable_ident"



	// $ANTLR start "variable_name"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:106:1: variable_name : IDENT ;
	public final void variable_name() throws  {
		int variable_name_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "variable_name");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(106, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 47) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:106:14: ( IDENT )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:106:17: IDENT
			{
			dbg.location(106,17);
			match(input,IDENT,FOLLOW_IDENT_in_variable_name741); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 47, variable_name_StartIndex); }

		}
		dbg.location(106, 21);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "variable_name");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "variable_name"



	// $ANTLR start "variable_decl_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:108:1: variable_decl_stmnt : type_denoter IDENT ( variable_init )? ;
	public final void variable_decl_stmnt() throws  {
		int variable_decl_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "variable_decl_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(108, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 48) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:108:20: ( type_denoter IDENT ( variable_init )? )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:108:22: type_denoter IDENT ( variable_init )?
			{
			dbg.location(108,22);
			pushFollow(FOLLOW_type_denoter_in_variable_decl_stmnt748);
			type_denoter();
			state._fsp--;
			if (state.failed) return;dbg.location(108,35);
			match(input,IDENT,FOLLOW_IDENT_in_variable_decl_stmnt750); if (state.failed) return;dbg.location(108,41);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:108:41: ( variable_init )?
			int alt30=2;
			try { dbg.enterSubRule(30);
			try { dbg.enterDecision(30, decisionCanBacktrack[30]);

			int LA30_0 = input.LA(1);
			if ( (LA30_0==50) ) {
				alt30=1;
			}
			} finally {dbg.exitDecision(30);}

			switch (alt30) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:108:41: variable_init
					{
					dbg.location(108,41);
					pushFollow(FOLLOW_variable_init_in_variable_decl_stmnt752);
					variable_init();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(30);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 48, variable_decl_stmnt_StartIndex); }

		}
		dbg.location(108, 54);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "variable_decl_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "variable_decl_stmnt"



	// $ANTLR start "variable_init"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:109:1: variable_init : ':=' expr ;
	public final void variable_init() throws  {
		int variable_init_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "variable_init");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(109, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 49) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:109:14: ( ':=' expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:109:18: ':=' expr
			{
			dbg.location(109,18);
			match(input,50,FOLLOW_50_in_variable_init761); if (state.failed) return;dbg.location(109,23);
			pushFollow(FOLLOW_expr_in_variable_init763);
			expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 49, variable_init_StartIndex); }

		}
		dbg.location(109, 26);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "variable_init");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "variable_init"



	// $ANTLR start "class_decl_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:112:1: class_decl_stmnt : ( class_modifier )? 'class' class_name ( type_inheritance )? class_body ;
	public final void class_decl_stmnt() throws  {
		int class_decl_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "class_decl_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(112, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 50) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:112:17: ( ( class_modifier )? 'class' class_name ( type_inheritance )? class_body )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:112:19: ( class_modifier )? 'class' class_name ( type_inheritance )? class_body
			{
			dbg.location(112,19);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:112:19: ( class_modifier )?
			int alt31=2;
			try { dbg.enterSubRule(31);
			try { dbg.enterDecision(31, decisionCanBacktrack[31]);

			int LA31_0 = input.LA(1);
			if ( (LA31_0==74) ) {
				alt31=1;
			}
			} finally {dbg.exitDecision(31);}

			switch (alt31) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:112:19: class_modifier
					{
					dbg.location(112,19);
					pushFollow(FOLLOW_class_modifier_in_class_decl_stmnt771);
					class_modifier();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(31);}
			dbg.location(112,35);
			match(input,68,FOLLOW_68_in_class_decl_stmnt774); if (state.failed) return;dbg.location(112,43);
			pushFollow(FOLLOW_class_name_in_class_decl_stmnt776);
			class_name();
			state._fsp--;
			if (state.failed) return;dbg.location(112,54);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:112:54: ( type_inheritance )?
			int alt32=2;
			try { dbg.enterSubRule(32);
			try { dbg.enterDecision(32, decisionCanBacktrack[32]);

			int LA32_0 = input.LA(1);
			if ( (LA32_0==49) ) {
				alt32=1;
			}
			} finally {dbg.exitDecision(32);}

			switch (alt32) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:112:54: type_inheritance
					{
					dbg.location(112,54);
					pushFollow(FOLLOW_type_inheritance_in_class_decl_stmnt778);
					type_inheritance();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(32);}
			dbg.location(112,72);
			pushFollow(FOLLOW_class_body_in_class_decl_stmnt781);
			class_body();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 50, class_decl_stmnt_StartIndex); }

		}
		dbg.location(112, 81);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "class_decl_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "class_decl_stmnt"



	// $ANTLR start "class_modifier"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:113:1: class_modifier : 'extern' ;
	public final void class_modifier() throws  {
		int class_modifier_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "class_modifier");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(113, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 51) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:113:15: ( 'extern' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:113:18: 'extern'
			{
			dbg.location(113,18);
			match(input,74,FOLLOW_74_in_class_modifier788); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 51, class_modifier_StartIndex); }

		}
		dbg.location(113, 25);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "class_modifier");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "class_modifier"



	// $ANTLR start "class_body"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:114:1: class_body : '{' ( decl_stmnt_list )? '}' ;
	public final void class_body() throws  {
		int class_body_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "class_body");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(114, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 52) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:114:11: ( '{' ( decl_stmnt_list )? '}' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:114:15: '{' ( decl_stmnt_list )? '}'
			{
			dbg.location(114,15);
			match(input,86,FOLLOW_86_in_class_body796); if (state.failed) return;dbg.location(114,19);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:114:19: ( decl_stmnt_list )?
			int alt33=2;
			try { dbg.enterSubRule(33);
			try { dbg.enterDecision(33, decisionCanBacktrack[33]);

			int LA33_0 = input.LA(1);
			if ( (LA33_0==BOOL_TYPE_DENOTER||LA33_0==FLOAT_TYPE_DENOTER||LA33_0==IDENT||LA33_0==INT_TYPE_DENOTER||LA33_0==61||LA33_0==68||(LA33_0 >= 73 && LA33_0 <= 75)||LA33_0==83) ) {
				alt33=1;
			}
			} finally {dbg.exitDecision(33);}

			switch (alt33) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:114:19: decl_stmnt_list
					{
					dbg.location(114,19);
					pushFollow(FOLLOW_decl_stmnt_list_in_class_body798);
					decl_stmnt_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(33);}
			dbg.location(114,36);
			match(input,89,FOLLOW_89_in_class_body801); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 52, class_body_StartIndex); }

		}
		dbg.location(114, 38);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "class_body");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "class_body"



	// $ANTLR start "class_name"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:115:1: class_name : IDENT ;
	public final void class_name() throws  {
		int class_name_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "class_name");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(115, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 53) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:115:11: ( IDENT )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:115:15: IDENT
			{
			dbg.location(115,15);
			match(input,IDENT,FOLLOW_IDENT_in_class_name809); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 53, class_name_StartIndex); }

		}
		dbg.location(115, 19);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "class_name");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "class_name"



	// $ANTLR start "type_inheritance"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:117:1: type_inheritance : ':' variable_ident ;
	public final void type_inheritance() throws  {
		int type_inheritance_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "type_inheritance");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(117, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 54) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:117:17: ( ':' variable_ident )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:117:20: ':' variable_ident
			{
			dbg.location(117,20);
			match(input,49,FOLLOW_49_in_type_inheritance817); if (state.failed) return;dbg.location(117,24);
			pushFollow(FOLLOW_variable_ident_in_type_inheritance819);
			variable_ident();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 54, type_inheritance_StartIndex); }

		}
		dbg.location(117, 37);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "type_inheritance");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "type_inheritance"



	// $ANTLR start "type_multi_inheritance"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:118:1: type_multi_inheritance : ':' variable_ident ( ',' variable_ident )* ;
	public final void type_multi_inheritance() throws  {
		int type_multi_inheritance_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "type_multi_inheritance");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(118, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 55) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:118:23: ( ':' variable_ident ( ',' variable_ident )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:118:25: ':' variable_ident ( ',' variable_ident )*
			{
			dbg.location(118,25);
			match(input,49,FOLLOW_49_in_type_multi_inheritance825); if (state.failed) return;dbg.location(118,29);
			pushFollow(FOLLOW_variable_ident_in_type_multi_inheritance827);
			variable_ident();
			state._fsp--;
			if (state.failed) return;dbg.location(118,44);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:118:44: ( ',' variable_ident )*
			try { dbg.enterSubRule(34);

			loop34:
			while (true) {
				int alt34=2;
				try { dbg.enterDecision(34, decisionCanBacktrack[34]);

				int LA34_0 = input.LA(1);
				if ( (LA34_0==40) ) {
					alt34=1;
				}

				} finally {dbg.exitDecision(34);}

				switch (alt34) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:118:45: ',' variable_ident
					{
					dbg.location(118,45);
					match(input,40,FOLLOW_40_in_type_multi_inheritance830); if (state.failed) return;dbg.location(118,49);
					pushFollow(FOLLOW_variable_ident_in_type_multi_inheritance832);
					variable_ident();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop34;
				}
			}
			} finally {dbg.exitSubRule(34);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 55, type_multi_inheritance_StartIndex); }

		}
		dbg.location(118, 64);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "type_multi_inheritance");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "type_multi_inheritance"



	// $ANTLR start "enum_decl_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:121:1: enum_decl_stmnt : 'enum' variable_name enum_body ;
	public final void enum_decl_stmnt() throws  {
		int enum_decl_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "enum_decl_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(121, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 56) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:121:16: ( 'enum' variable_name enum_body )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:121:18: 'enum' variable_name enum_body
			{
			dbg.location(121,18);
			match(input,73,FOLLOW_73_in_enum_decl_stmnt842); if (state.failed) return;dbg.location(121,25);
			pushFollow(FOLLOW_variable_name_in_enum_decl_stmnt844);
			variable_name();
			state._fsp--;
			if (state.failed) return;dbg.location(121,39);
			pushFollow(FOLLOW_enum_body_in_enum_decl_stmnt846);
			enum_body();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 56, enum_decl_stmnt_StartIndex); }

		}
		dbg.location(121, 47);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "enum_decl_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "enum_decl_stmnt"



	// $ANTLR start "enum_body"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:122:1: enum_body : '{' ( enum_entry_list )? '}' ;
	public final void enum_body() throws  {
		int enum_body_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "enum_body");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(122, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 57) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:122:10: ( '{' ( enum_entry_list )? '}' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:122:14: '{' ( enum_entry_list )? '}'
			{
			dbg.location(122,14);
			match(input,86,FOLLOW_86_in_enum_body854); if (state.failed) return;dbg.location(122,18);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:122:18: ( enum_entry_list )?
			int alt35=2;
			try { dbg.enterSubRule(35);
			try { dbg.enterDecision(35, decisionCanBacktrack[35]);

			int LA35_0 = input.LA(1);
			if ( (LA35_0==IDENT) ) {
				alt35=1;
			}
			} finally {dbg.exitDecision(35);}

			switch (alt35) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:122:18: enum_entry_list
					{
					dbg.location(122,18);
					pushFollow(FOLLOW_enum_entry_list_in_enum_body856);
					enum_entry_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(35);}
			dbg.location(122,35);
			match(input,89,FOLLOW_89_in_enum_body859); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 57, enum_body_StartIndex); }

		}
		dbg.location(122, 37);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "enum_body");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "enum_body"



	// $ANTLR start "enum_entry_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:123:1: enum_entry_list : enum_entry ( ',' enum_entry )* ;
	public final void enum_entry_list() throws  {
		int enum_entry_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "enum_entry_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(123, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 58) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:123:16: ( enum_entry ( ',' enum_entry )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:123:18: enum_entry ( ',' enum_entry )*
			{
			dbg.location(123,18);
			pushFollow(FOLLOW_enum_entry_in_enum_entry_list865);
			enum_entry();
			state._fsp--;
			if (state.failed) return;dbg.location(123,29);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:123:29: ( ',' enum_entry )*
			try { dbg.enterSubRule(36);

			loop36:
			while (true) {
				int alt36=2;
				try { dbg.enterDecision(36, decisionCanBacktrack[36]);

				int LA36_0 = input.LA(1);
				if ( (LA36_0==40) ) {
					alt36=1;
				}

				} finally {dbg.exitDecision(36);}

				switch (alt36) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:123:30: ',' enum_entry
					{
					dbg.location(123,30);
					match(input,40,FOLLOW_40_in_enum_entry_list868); if (state.failed) return;dbg.location(123,34);
					pushFollow(FOLLOW_enum_entry_in_enum_entry_list870);
					enum_entry();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop36;
				}
			}
			} finally {dbg.exitSubRule(36);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 58, enum_entry_list_StartIndex); }

		}
		dbg.location(123, 45);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "enum_entry_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "enum_entry_list"



	// $ANTLR start "enum_entry"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:124:1: enum_entry : enum_entry_name ( ':=' expr )? ;
	public final void enum_entry() throws  {
		int enum_entry_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "enum_entry");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(124, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 59) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:124:11: ( enum_entry_name ( ':=' expr )? )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:124:15: enum_entry_name ( ':=' expr )?
			{
			dbg.location(124,15);
			pushFollow(FOLLOW_enum_entry_name_in_enum_entry880);
			enum_entry_name();
			state._fsp--;
			if (state.failed) return;dbg.location(124,31);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:124:31: ( ':=' expr )?
			int alt37=2;
			try { dbg.enterSubRule(37);
			try { dbg.enterDecision(37, decisionCanBacktrack[37]);

			int LA37_0 = input.LA(1);
			if ( (LA37_0==50) ) {
				alt37=1;
			}
			} finally {dbg.exitDecision(37);}

			switch (alt37) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:124:32: ':=' expr
					{
					dbg.location(124,32);
					match(input,50,FOLLOW_50_in_enum_entry883); if (state.failed) return;dbg.location(124,37);
					pushFollow(FOLLOW_expr_in_enum_entry885);
					expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(37);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 59, enum_entry_StartIndex); }

		}
		dbg.location(124, 42);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "enum_entry");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "enum_entry"



	// $ANTLR start "enum_entry_name"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:125:1: enum_entry_name : IDENT ;
	public final void enum_entry_name() throws  {
		int enum_entry_name_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "enum_entry_name");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(125, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 60) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:125:16: ( IDENT )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:125:18: IDENT
			{
			dbg.location(125,18);
			match(input,IDENT,FOLLOW_IDENT_in_enum_entry_name893); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 60, enum_entry_name_StartIndex); }

		}
		dbg.location(125, 22);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "enum_entry_name");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "enum_entry_name"



	// $ANTLR start "flags_decl_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:128:1: flags_decl_stmnt : 'flags' variable_name ( type_multi_inheritance )? flags_body ;
	public final void flags_decl_stmnt() throws  {
		int flags_decl_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "flags_decl_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(128, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 61) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:128:17: ( 'flags' variable_name ( type_multi_inheritance )? flags_body )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:128:19: 'flags' variable_name ( type_multi_inheritance )? flags_body
			{
			dbg.location(128,19);
			match(input,75,FOLLOW_75_in_flags_decl_stmnt901); if (state.failed) return;dbg.location(128,27);
			pushFollow(FOLLOW_variable_name_in_flags_decl_stmnt903);
			variable_name();
			state._fsp--;
			if (state.failed) return;dbg.location(128,41);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:128:41: ( type_multi_inheritance )?
			int alt38=2;
			try { dbg.enterSubRule(38);
			try { dbg.enterDecision(38, decisionCanBacktrack[38]);

			int LA38_0 = input.LA(1);
			if ( (LA38_0==49) ) {
				alt38=1;
			}
			} finally {dbg.exitDecision(38);}

			switch (alt38) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:128:41: type_multi_inheritance
					{
					dbg.location(128,41);
					pushFollow(FOLLOW_type_multi_inheritance_in_flags_decl_stmnt905);
					type_multi_inheritance();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(38);}
			dbg.location(128,65);
			pushFollow(FOLLOW_flags_body_in_flags_decl_stmnt908);
			flags_body();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 61, flags_decl_stmnt_StartIndex); }

		}
		dbg.location(128, 74);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "flags_decl_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "flags_decl_stmnt"



	// $ANTLR start "flags_body"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:129:1: flags_body : '{' ( flags_entry_list )? '}' ;
	public final void flags_body() throws  {
		int flags_body_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "flags_body");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(129, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 62) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:129:11: ( '{' ( flags_entry_list )? '}' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:129:15: '{' ( flags_entry_list )? '}'
			{
			dbg.location(129,15);
			match(input,86,FOLLOW_86_in_flags_body916); if (state.failed) return;dbg.location(129,19);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:129:19: ( flags_entry_list )?
			int alt39=2;
			try { dbg.enterSubRule(39);
			try { dbg.enterDecision(39, decisionCanBacktrack[39]);

			int LA39_0 = input.LA(1);
			if ( (LA39_0==IDENT) ) {
				alt39=1;
			}
			} finally {dbg.exitDecision(39);}

			switch (alt39) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:129:19: flags_entry_list
					{
					dbg.location(129,19);
					pushFollow(FOLLOW_flags_entry_list_in_flags_body918);
					flags_entry_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(39);}
			dbg.location(129,37);
			match(input,89,FOLLOW_89_in_flags_body921); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 62, flags_body_StartIndex); }

		}
		dbg.location(129, 39);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "flags_body");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "flags_body"



	// $ANTLR start "flags_entry_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:130:1: flags_entry_list : flags_entry ( ',' flags_entry )* ;
	public final void flags_entry_list() throws  {
		int flags_entry_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "flags_entry_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(130, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 63) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:130:17: ( flags_entry ( ',' flags_entry )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:130:19: flags_entry ( ',' flags_entry )*
			{
			dbg.location(130,19);
			pushFollow(FOLLOW_flags_entry_in_flags_entry_list927);
			flags_entry();
			state._fsp--;
			if (state.failed) return;dbg.location(130,31);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:130:31: ( ',' flags_entry )*
			try { dbg.enterSubRule(40);

			loop40:
			while (true) {
				int alt40=2;
				try { dbg.enterDecision(40, decisionCanBacktrack[40]);

				int LA40_0 = input.LA(1);
				if ( (LA40_0==40) ) {
					alt40=1;
				}

				} finally {dbg.exitDecision(40);}

				switch (alt40) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:130:32: ',' flags_entry
					{
					dbg.location(130,32);
					match(input,40,FOLLOW_40_in_flags_entry_list930); if (state.failed) return;dbg.location(130,36);
					pushFollow(FOLLOW_flags_entry_in_flags_entry_list932);
					flags_entry();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop40;
				}
			}
			} finally {dbg.exitSubRule(40);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 63, flags_entry_list_StartIndex); }

		}
		dbg.location(130, 48);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "flags_entry_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "flags_entry_list"



	// $ANTLR start "flags_entry"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:131:1: flags_entry : IDENT ;
	public final void flags_entry() throws  {
		int flags_entry_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "flags_entry");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(131, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 64) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:131:12: ( IDENT )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:131:15: IDENT
			{
			dbg.location(131,15);
			match(input,IDENT,FOLLOW_IDENT_in_flags_entry941); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 64, flags_entry_StartIndex); }

		}
		dbg.location(131, 19);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "flags_entry");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "flags_entry"



	// $ANTLR start "function_decl_stmnt"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:134:1: function_decl_stmnt : ( attribute_prefix )? function_head code_block ;
	public final void function_decl_stmnt() throws  {
		int function_decl_stmnt_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "function_decl_stmnt");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(134, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 65) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:134:20: ( ( attribute_prefix )? function_head code_block )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:134:22: ( attribute_prefix )? function_head code_block
			{
			dbg.location(134,22);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:134:22: ( attribute_prefix )?
			int alt41=2;
			try { dbg.enterSubRule(41);
			try { dbg.enterDecision(41, decisionCanBacktrack[41]);

			int LA41_0 = input.LA(1);
			if ( (LA41_0==61) ) {
				alt41=1;
			}
			} finally {dbg.exitDecision(41);}

			switch (alt41) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:134:22: attribute_prefix
					{
					dbg.location(134,22);
					pushFollow(FOLLOW_attribute_prefix_in_function_decl_stmnt949);
					attribute_prefix();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(41);}
			dbg.location(134,40);
			pushFollow(FOLLOW_function_head_in_function_decl_stmnt952);
			function_head();
			state._fsp--;
			if (state.failed) return;dbg.location(134,54);
			pushFollow(FOLLOW_code_block_in_function_decl_stmnt954);
			code_block();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 65, function_decl_stmnt_StartIndex); }

		}
		dbg.location(134, 63);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "function_decl_stmnt");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "function_decl_stmnt"



	// $ANTLR start "function_head"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:135:1: function_head : ( function_modifier )? type_denoter variable_name '(' ( parameter_list )? ')' ;
	public final void function_head() throws  {
		int function_head_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "function_head");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(135, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 66) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:135:14: ( ( function_modifier )? type_denoter variable_name '(' ( parameter_list )? ')' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:135:18: ( function_modifier )? type_denoter variable_name '(' ( parameter_list )? ')'
			{
			dbg.location(135,18);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:135:18: ( function_modifier )?
			int alt42=2;
			try { dbg.enterSubRule(42);
			try { dbg.enterDecision(42, decisionCanBacktrack[42]);

			int LA42_0 = input.LA(1);
			if ( (LA42_0==83) ) {
				alt42=1;
			}
			} finally {dbg.exitDecision(42);}

			switch (alt42) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:135:18: function_modifier
					{
					dbg.location(135,18);
					pushFollow(FOLLOW_function_modifier_in_function_head962);
					function_modifier();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(42);}
			dbg.location(135,37);
			pushFollow(FOLLOW_type_denoter_in_function_head965);
			type_denoter();
			state._fsp--;
			if (state.failed) return;dbg.location(135,50);
			pushFollow(FOLLOW_variable_name_in_function_head967);
			variable_name();
			state._fsp--;
			if (state.failed) return;dbg.location(135,64);
			match(input,33,FOLLOW_33_in_function_head969); if (state.failed) return;dbg.location(135,68);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:135:68: ( parameter_list )?
			int alt43=2;
			try { dbg.enterSubRule(43);
			try { dbg.enterDecision(43, decisionCanBacktrack[43]);

			int LA43_0 = input.LA(1);
			if ( (LA43_0==BOOL_TYPE_DENOTER||LA43_0==FLOAT_TYPE_DENOTER||LA43_0==IDENT||LA43_0==INT_TYPE_DENOTER) ) {
				alt43=1;
			}
			} finally {dbg.exitDecision(43);}

			switch (alt43) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:135:68: parameter_list
					{
					dbg.location(135,68);
					pushFollow(FOLLOW_parameter_list_in_function_head971);
					parameter_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(43);}
			dbg.location(135,84);
			match(input,34,FOLLOW_34_in_function_head974); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 66, function_head_StartIndex); }

		}
		dbg.location(135, 86);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "function_head");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "function_head"



	// $ANTLR start "function_modifier"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:136:1: function_modifier : 'static' ;
	public final void function_modifier() throws  {
		int function_modifier_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "function_modifier");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(136, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 67) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:136:18: ( 'static' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:136:21: 'static'
			{
			dbg.location(136,21);
			match(input,83,FOLLOW_83_in_function_modifier981); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 67, function_modifier_StartIndex); }

		}
		dbg.location(136, 28);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "function_modifier");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "function_modifier"



	// $ANTLR start "parameter_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:137:1: parameter_list : parameter ( ',' parameter )* ;
	public final void parameter_list() throws  {
		int parameter_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "parameter_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(137, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 68) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:137:15: ( parameter ( ',' parameter )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:137:19: parameter ( ',' parameter )*
			{
			dbg.location(137,19);
			pushFollow(FOLLOW_parameter_in_parameter_list989);
			parameter();
			state._fsp--;
			if (state.failed) return;dbg.location(137,29);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:137:29: ( ',' parameter )*
			try { dbg.enterSubRule(44);

			loop44:
			while (true) {
				int alt44=2;
				try { dbg.enterDecision(44, decisionCanBacktrack[44]);

				int LA44_0 = input.LA(1);
				if ( (LA44_0==40) ) {
					alt44=1;
				}

				} finally {dbg.exitDecision(44);}

				switch (alt44) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:137:30: ',' parameter
					{
					dbg.location(137,30);
					match(input,40,FOLLOW_40_in_parameter_list992); if (state.failed) return;dbg.location(137,34);
					pushFollow(FOLLOW_parameter_in_parameter_list994);
					parameter();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop44;
				}
			}
			} finally {dbg.exitSubRule(44);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 68, parameter_list_StartIndex); }

		}
		dbg.location(137, 44);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "parameter_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "parameter_list"



	// $ANTLR start "parameter"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:138:1: parameter : type_denoter variable_name ( ':=' expr )? ;
	public final void parameter() throws  {
		int parameter_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "parameter");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(138, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 69) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:138:10: ( type_denoter variable_name ( ':=' expr )? )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:138:15: type_denoter variable_name ( ':=' expr )?
			{
			dbg.location(138,15);
			pushFollow(FOLLOW_type_denoter_in_parameter1005);
			type_denoter();
			state._fsp--;
			if (state.failed) return;dbg.location(138,28);
			pushFollow(FOLLOW_variable_name_in_parameter1007);
			variable_name();
			state._fsp--;
			if (state.failed) return;dbg.location(138,42);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:138:42: ( ':=' expr )?
			int alt45=2;
			try { dbg.enterSubRule(45);
			try { dbg.enterDecision(45, decisionCanBacktrack[45]);

			int LA45_0 = input.LA(1);
			if ( (LA45_0==50) ) {
				alt45=1;
			}
			} finally {dbg.exitDecision(45);}

			switch (alt45) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:138:43: ':=' expr
					{
					dbg.location(138,43);
					match(input,50,FOLLOW_50_in_parameter1010); if (state.failed) return;dbg.location(138,48);
					pushFollow(FOLLOW_expr_in_parameter1012);
					expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(45);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 69, parameter_StartIndex); }

		}
		dbg.location(138, 53);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "parameter");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "parameter"



	// $ANTLR start "expr_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:141:1: expr_list : expr ( ',' expr )* ;
	public final void expr_list() throws  {
		int expr_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "expr_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(141, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 70) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:141:10: ( expr ( ',' expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:141:12: expr ( ',' expr )*
			{
			dbg.location(141,12);
			pushFollow(FOLLOW_expr_in_expr_list1022);
			expr();
			state._fsp--;
			if (state.failed) return;dbg.location(141,17);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:141:17: ( ',' expr )*
			try { dbg.enterSubRule(46);

			loop46:
			while (true) {
				int alt46=2;
				try { dbg.enterDecision(46, decisionCanBacktrack[46]);

				int LA46_0 = input.LA(1);
				if ( (LA46_0==40) ) {
					alt46=1;
				}

				} finally {dbg.exitDecision(46);}

				switch (alt46) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:141:18: ',' expr
					{
					dbg.location(141,18);
					match(input,40,FOLLOW_40_in_expr_list1025); if (state.failed) return;dbg.location(141,22);
					pushFollow(FOLLOW_expr_in_expr_list1027);
					expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop46;
				}
			}
			} finally {dbg.exitSubRule(46);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 70, expr_list_StartIndex); }

		}
		dbg.location(141, 27);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "expr_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "expr_list"



	// $ANTLR start "expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:143:1: expr : ( arithmetic_expr | init_list_expr );
	public final void expr() throws  {
		int expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(143, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 71) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:143:6: ( arithmetic_expr | init_list_expr )
			int alt47=2;
			try { dbg.enterDecision(47, decisionCanBacktrack[47]);

			int LA47_0 = input.LA(1);
			if ( (LA47_0==IDENT||LA47_0==33||LA47_0==41||LA47_0==80||LA47_0==90) ) {
				alt47=1;
			}
			else if ( (LA47_0==86) ) {
				alt47=2;
			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 47, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(47);}

			switch (alt47) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:143:8: arithmetic_expr
					{
					dbg.location(143,8);
					pushFollow(FOLLOW_arithmetic_expr_in_expr1037);
					arithmetic_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:144:5: init_list_expr
					{
					dbg.location(144,5);
					pushFollow(FOLLOW_init_list_expr_in_expr1043);
					init_list_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 71, expr_StartIndex); }

		}
		dbg.location(144, 18);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "expr"



	// $ANTLR start "init_list_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:146:1: init_list_expr : '{' ( expr_list )? '}' ;
	public final void init_list_expr() throws  {
		int init_list_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "init_list_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(146, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 72) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:146:15: ( '{' ( expr_list )? '}' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:146:17: '{' ( expr_list )? '}'
			{
			dbg.location(146,17);
			match(input,86,FOLLOW_86_in_init_list_expr1050); if (state.failed) return;dbg.location(146,21);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:146:21: ( expr_list )?
			int alt48=2;
			try { dbg.enterSubRule(48);
			try { dbg.enterDecision(48, decisionCanBacktrack[48]);

			int LA48_0 = input.LA(1);
			if ( (LA48_0==IDENT||LA48_0==33||LA48_0==41||LA48_0==80||LA48_0==86||LA48_0==90) ) {
				alt48=1;
			}
			} finally {dbg.exitDecision(48);}

			switch (alt48) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:146:21: expr_list
					{
					dbg.location(146,21);
					pushFollow(FOLLOW_expr_list_in_init_list_expr1052);
					expr_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(48);}
			dbg.location(146,32);
			match(input,89,FOLLOW_89_in_init_list_expr1055); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 72, init_list_expr_StartIndex); }

		}
		dbg.location(146, 34);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "init_list_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "init_list_expr"



	// $ANTLR start "arithmetic_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:148:1: arithmetic_expr : logic_or_expr ;
	public final void arithmetic_expr() throws  {
		int arithmetic_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "arithmetic_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(148, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 73) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:148:16: ( logic_or_expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:148:18: logic_or_expr
			{
			dbg.location(148,18);
			pushFollow(FOLLOW_logic_or_expr_in_arithmetic_expr1062);
			logic_or_expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 73, arithmetic_expr_StartIndex); }

		}
		dbg.location(148, 30);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "arithmetic_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "arithmetic_expr"



	// $ANTLR start "logic_or_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:149:1: logic_or_expr : logic_and_expr ( 'or' logic_and_expr )* ;
	public final void logic_or_expr() throws  {
		int logic_or_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "logic_or_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(149, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 74) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:149:14: ( logic_and_expr ( 'or' logic_and_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:149:17: logic_and_expr ( 'or' logic_and_expr )*
			{
			dbg.location(149,17);
			pushFollow(FOLLOW_logic_and_expr_in_logic_or_expr1069);
			logic_and_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(149,32);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:149:32: ( 'or' logic_and_expr )*
			try { dbg.enterSubRule(49);

			loop49:
			while (true) {
				int alt49=2;
				try { dbg.enterDecision(49, decisionCanBacktrack[49]);

				int LA49_0 = input.LA(1);
				if ( (LA49_0==81) ) {
					alt49=1;
				}

				} finally {dbg.exitDecision(49);}

				switch (alt49) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:149:33: 'or' logic_and_expr
					{
					dbg.location(149,33);
					match(input,81,FOLLOW_81_in_logic_or_expr1072); if (state.failed) return;dbg.location(149,38);
					pushFollow(FOLLOW_logic_and_expr_in_logic_or_expr1074);
					logic_and_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop49;
				}
			}
			} finally {dbg.exitSubRule(49);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 74, logic_or_expr_StartIndex); }

		}
		dbg.location(149, 53);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "logic_or_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "logic_or_expr"



	// $ANTLR start "logic_and_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:150:1: logic_and_expr : bitwise_or_expr ( 'and' bitwise_or_expr )* ;
	public final void logic_and_expr() throws  {
		int logic_and_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "logic_and_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(150, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 75) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:150:15: ( bitwise_or_expr ( 'and' bitwise_or_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:150:18: bitwise_or_expr ( 'and' bitwise_or_expr )*
			{
			dbg.location(150,18);
			pushFollow(FOLLOW_bitwise_or_expr_in_logic_and_expr1083);
			bitwise_or_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(150,34);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:150:34: ( 'and' bitwise_or_expr )*
			try { dbg.enterSubRule(50);

			loop50:
			while (true) {
				int alt50=2;
				try { dbg.enterDecision(50, decisionCanBacktrack[50]);

				int LA50_0 = input.LA(1);
				if ( (LA50_0==65) ) {
					alt50=1;
				}

				} finally {dbg.exitDecision(50);}

				switch (alt50) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:150:35: 'and' bitwise_or_expr
					{
					dbg.location(150,35);
					match(input,65,FOLLOW_65_in_logic_and_expr1086); if (state.failed) return;dbg.location(150,41);
					pushFollow(FOLLOW_bitwise_or_expr_in_logic_and_expr1088);
					bitwise_or_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop50;
				}
			}
			} finally {dbg.exitSubRule(50);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 75, logic_and_expr_StartIndex); }

		}
		dbg.location(150, 57);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "logic_and_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "logic_and_expr"



	// $ANTLR start "bitwise_or_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:151:1: bitwise_or_expr : bitwise_xor_expr ( '|' bitwise_xor_expr )* ;
	public final void bitwise_or_expr() throws  {
		int bitwise_or_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "bitwise_or_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(151, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 76) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:151:16: ( bitwise_xor_expr ( '|' bitwise_xor_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:151:18: bitwise_xor_expr ( '|' bitwise_xor_expr )*
			{
			dbg.location(151,18);
			pushFollow(FOLLOW_bitwise_xor_expr_in_bitwise_or_expr1096);
			bitwise_xor_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(151,35);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:151:35: ( '|' bitwise_xor_expr )*
			try { dbg.enterSubRule(51);

			loop51:
			while (true) {
				int alt51=2;
				try { dbg.enterDecision(51, decisionCanBacktrack[51]);

				int LA51_0 = input.LA(1);
				if ( (LA51_0==87) ) {
					alt51=1;
				}

				} finally {dbg.exitDecision(51);}

				switch (alt51) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:151:36: '|' bitwise_xor_expr
					{
					dbg.location(151,36);
					match(input,87,FOLLOW_87_in_bitwise_or_expr1099); if (state.failed) return;dbg.location(151,40);
					pushFollow(FOLLOW_bitwise_xor_expr_in_bitwise_or_expr1101);
					bitwise_xor_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop51;
				}
			}
			} finally {dbg.exitSubRule(51);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 76, bitwise_or_expr_StartIndex); }

		}
		dbg.location(151, 57);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "bitwise_or_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "bitwise_or_expr"



	// $ANTLR start "bitwise_xor_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:152:1: bitwise_xor_expr : bitwise_and_expr ( '^' bitwise_and_expr )* ;
	public final void bitwise_xor_expr() throws  {
		int bitwise_xor_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "bitwise_xor_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(152, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 77) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:152:17: ( bitwise_and_expr ( '^' bitwise_and_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:152:19: bitwise_and_expr ( '^' bitwise_and_expr )*
			{
			dbg.location(152,19);
			pushFollow(FOLLOW_bitwise_and_expr_in_bitwise_xor_expr1109);
			bitwise_and_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(152,36);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:152:36: ( '^' bitwise_and_expr )*
			try { dbg.enterSubRule(52);

			loop52:
			while (true) {
				int alt52=2;
				try { dbg.enterDecision(52, decisionCanBacktrack[52]);

				int LA52_0 = input.LA(1);
				if ( (LA52_0==63) ) {
					alt52=1;
				}

				} finally {dbg.exitDecision(52);}

				switch (alt52) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:152:37: '^' bitwise_and_expr
					{
					dbg.location(152,37);
					match(input,63,FOLLOW_63_in_bitwise_xor_expr1112); if (state.failed) return;dbg.location(152,41);
					pushFollow(FOLLOW_bitwise_and_expr_in_bitwise_xor_expr1114);
					bitwise_and_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop52;
				}
			}
			} finally {dbg.exitSubRule(52);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 77, bitwise_xor_expr_StartIndex); }

		}
		dbg.location(152, 58);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "bitwise_xor_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "bitwise_xor_expr"



	// $ANTLR start "bitwise_and_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:153:1: bitwise_and_expr : equality_expr ( '&' equality_expr )* ;
	public final void bitwise_and_expr() throws  {
		int bitwise_and_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "bitwise_and_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(153, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 78) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:153:17: ( equality_expr ( '&' equality_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:153:19: equality_expr ( '&' equality_expr )*
			{
			dbg.location(153,19);
			pushFollow(FOLLOW_equality_expr_in_bitwise_and_expr1122);
			equality_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(153,33);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:153:33: ( '&' equality_expr )*
			try { dbg.enterSubRule(53);

			loop53:
			while (true) {
				int alt53=2;
				try { dbg.enterDecision(53, decisionCanBacktrack[53]);

				int LA53_0 = input.LA(1);
				if ( (LA53_0==31) ) {
					alt53=1;
				}

				} finally {dbg.exitDecision(53);}

				switch (alt53) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:153:34: '&' equality_expr
					{
					dbg.location(153,34);
					match(input,31,FOLLOW_31_in_bitwise_and_expr1125); if (state.failed) return;dbg.location(153,38);
					pushFollow(FOLLOW_equality_expr_in_bitwise_and_expr1127);
					equality_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop53;
				}
			}
			} finally {dbg.exitSubRule(53);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 78, bitwise_and_expr_StartIndex); }

		}
		dbg.location(153, 52);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "bitwise_and_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "bitwise_and_expr"



	// $ANTLR start "equality_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:154:1: equality_expr : relation_expr ( ( '=' | '!=' ) relation_expr )* ;
	public final void equality_expr() throws  {
		int equality_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "equality_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(154, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 79) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:154:14: ( relation_expr ( ( '=' | '!=' ) relation_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:154:17: relation_expr ( ( '=' | '!=' ) relation_expr )*
			{
			dbg.location(154,17);
			pushFollow(FOLLOW_relation_expr_in_equality_expr1136);
			relation_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(154,31);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:154:31: ( ( '=' | '!=' ) relation_expr )*
			try { dbg.enterSubRule(54);

			loop54:
			while (true) {
				int alt54=2;
				try { dbg.enterDecision(54, decisionCanBacktrack[54]);

				int LA54_0 = input.LA(1);
				if ( (LA54_0==28||LA54_0==56) ) {
					alt54=1;
				}

				} finally {dbg.exitDecision(54);}

				switch (alt54) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:154:32: ( '=' | '!=' ) relation_expr
					{
					dbg.location(154,32);
					if ( input.LA(1)==28||input.LA(1)==56 ) {
						input.consume();
						state.errorRecovery=false;
						state.failed=false;
					}
					else {
						if (state.backtracking>0) {state.failed=true; return;}
						MismatchedSetException mse = new MismatchedSetException(null,input);
						dbg.recognitionException(mse);
						throw mse;
					}dbg.location(154,45);
					pushFollow(FOLLOW_relation_expr_in_equality_expr1147);
					relation_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop54;
				}
			}
			} finally {dbg.exitSubRule(54);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 79, equality_expr_StartIndex); }

		}
		dbg.location(154, 59);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "equality_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "equality_expr"



	// $ANTLR start "relation_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:155:1: relation_expr : shift_expr ( ( '<' | '>' | '<=' | '>=' ) shift_expr )* ;
	public final void relation_expr() throws  {
		int relation_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "relation_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(155, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 80) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:155:14: ( shift_expr ( ( '<' | '>' | '<=' | '>=' ) shift_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:155:17: shift_expr ( ( '<' | '>' | '<=' | '>=' ) shift_expr )*
			{
			dbg.location(155,17);
			pushFollow(FOLLOW_shift_expr_in_relation_expr1156);
			shift_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(155,28);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:155:28: ( ( '<' | '>' | '<=' | '>=' ) shift_expr )*
			try { dbg.enterSubRule(55);

			loop55:
			while (true) {
				int alt55=2;
				try { dbg.enterDecision(55, decisionCanBacktrack[55]);

				int LA55_0 = input.LA(1);
				if ( (LA55_0==52||LA55_0==55||(LA55_0 >= 57 && LA55_0 <= 58)) ) {
					alt55=1;
				}

				} finally {dbg.exitDecision(55);}

				switch (alt55) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:155:29: ( '<' | '>' | '<=' | '>=' ) shift_expr
					{
					dbg.location(155,29);
					if ( input.LA(1)==52||input.LA(1)==55||(input.LA(1) >= 57 && input.LA(1) <= 58) ) {
						input.consume();
						state.errorRecovery=false;
						state.failed=false;
					}
					else {
						if (state.backtracking>0) {state.failed=true; return;}
						MismatchedSetException mse = new MismatchedSetException(null,input);
						dbg.recognitionException(mse);
						throw mse;
					}dbg.location(155,55);
					pushFollow(FOLLOW_shift_expr_in_relation_expr1175);
					shift_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop55;
				}
			}
			} finally {dbg.exitSubRule(55);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 80, relation_expr_StartIndex); }

		}
		dbg.location(155, 66);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "relation_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "relation_expr"



	// $ANTLR start "shift_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:156:1: shift_expr : add_expr ( ( '<<' | '>>' ) add_expr )* ;
	public final void shift_expr() throws  {
		int shift_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "shift_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(156, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 81) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:156:11: ( add_expr ( ( '<<' | '>>' ) add_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:156:15: add_expr ( ( '<<' | '>>' ) add_expr )*
			{
			dbg.location(156,15);
			pushFollow(FOLLOW_add_expr_in_shift_expr1185);
			add_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(156,24);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:156:24: ( ( '<<' | '>>' ) add_expr )*
			try { dbg.enterSubRule(56);

			loop56:
			while (true) {
				int alt56=2;
				try { dbg.enterDecision(56, decisionCanBacktrack[56]);

				int LA56_0 = input.LA(1);
				if ( (LA56_0==53||LA56_0==59) ) {
					alt56=1;
				}

				} finally {dbg.exitDecision(56);}

				switch (alt56) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:156:25: ( '<<' | '>>' ) add_expr
					{
					dbg.location(156,25);
					if ( input.LA(1)==53||input.LA(1)==59 ) {
						input.consume();
						state.errorRecovery=false;
						state.failed=false;
					}
					else {
						if (state.backtracking>0) {state.failed=true; return;}
						MismatchedSetException mse = new MismatchedSetException(null,input);
						dbg.recognitionException(mse);
						throw mse;
					}dbg.location(156,39);
					pushFollow(FOLLOW_add_expr_in_shift_expr1196);
					add_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop56;
				}
			}
			} finally {dbg.exitSubRule(56);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 81, shift_expr_StartIndex); }

		}
		dbg.location(156, 48);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "shift_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "shift_expr"



	// $ANTLR start "add_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:157:1: add_expr : sub_expr ( '+' sub_expr )* ;
	public final void add_expr() throws  {
		int add_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "add_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(157, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 82) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:157:9: ( sub_expr ( '+' sub_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:157:13: sub_expr ( '+' sub_expr )*
			{
			dbg.location(157,13);
			pushFollow(FOLLOW_sub_expr_in_add_expr1206);
			sub_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(157,22);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:157:22: ( '+' sub_expr )*
			try { dbg.enterSubRule(57);

			loop57:
			while (true) {
				int alt57=2;
				try { dbg.enterDecision(57, decisionCanBacktrack[57]);

				int LA57_0 = input.LA(1);
				if ( (LA57_0==37) ) {
					alt57=1;
				}

				} finally {dbg.exitDecision(57);}

				switch (alt57) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:157:23: '+' sub_expr
					{
					dbg.location(157,23);
					match(input,37,FOLLOW_37_in_add_expr1209); if (state.failed) return;dbg.location(157,27);
					pushFollow(FOLLOW_sub_expr_in_add_expr1211);
					sub_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop57;
				}
			}
			} finally {dbg.exitSubRule(57);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 82, add_expr_StartIndex); }

		}
		dbg.location(157, 36);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "add_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "add_expr"



	// $ANTLR start "sub_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:158:1: sub_expr : mul_expr ( '-' mul_expr )* ;
	public final void sub_expr() throws  {
		int sub_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "sub_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(158, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 83) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:158:9: ( mul_expr ( '-' mul_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:158:13: mul_expr ( '-' mul_expr )*
			{
			dbg.location(158,13);
			pushFollow(FOLLOW_mul_expr_in_sub_expr1221);
			mul_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(158,22);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:158:22: ( '-' mul_expr )*
			try { dbg.enterSubRule(58);

			loop58:
			while (true) {
				int alt58=2;
				try { dbg.enterDecision(58, decisionCanBacktrack[58]);

				int LA58_0 = input.LA(1);
				if ( (LA58_0==41) ) {
					alt58=1;
				}

				} finally {dbg.exitDecision(58);}

				switch (alt58) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:158:23: '-' mul_expr
					{
					dbg.location(158,23);
					match(input,41,FOLLOW_41_in_sub_expr1224); if (state.failed) return;dbg.location(158,27);
					pushFollow(FOLLOW_mul_expr_in_sub_expr1226);
					mul_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop58;
				}
			}
			} finally {dbg.exitSubRule(58);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 83, sub_expr_StartIndex); }

		}
		dbg.location(158, 36);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "sub_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "sub_expr"



	// $ANTLR start "mul_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:159:1: mul_expr : div_expr ( '*' div_expr )* ;
	public final void mul_expr() throws  {
		int mul_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "mul_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(159, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 84) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:159:9: ( div_expr ( '*' div_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:159:13: div_expr ( '*' div_expr )*
			{
			dbg.location(159,13);
			pushFollow(FOLLOW_div_expr_in_mul_expr1236);
			div_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(159,22);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:159:22: ( '*' div_expr )*
			try { dbg.enterSubRule(59);

			loop59:
			while (true) {
				int alt59=2;
				try { dbg.enterDecision(59, decisionCanBacktrack[59]);

				int LA59_0 = input.LA(1);
				if ( (LA59_0==35) ) {
					alt59=1;
				}

				} finally {dbg.exitDecision(59);}

				switch (alt59) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:159:23: '*' div_expr
					{
					dbg.location(159,23);
					match(input,35,FOLLOW_35_in_mul_expr1239); if (state.failed) return;dbg.location(159,27);
					pushFollow(FOLLOW_div_expr_in_mul_expr1241);
					div_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop59;
				}
			}
			} finally {dbg.exitSubRule(59);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 84, mul_expr_StartIndex); }

		}
		dbg.location(159, 36);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "mul_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "mul_expr"



	// $ANTLR start "div_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:160:1: div_expr : value_expr ( ( '/' | '%' ) value_expr )* ;
	public final void div_expr() throws  {
		int div_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "div_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(160, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 85) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:160:9: ( value_expr ( ( '/' | '%' ) value_expr )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:160:13: value_expr ( ( '/' | '%' ) value_expr )*
			{
			dbg.location(160,13);
			pushFollow(FOLLOW_value_expr_in_div_expr1251);
			value_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(160,24);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:160:24: ( ( '/' | '%' ) value_expr )*
			try { dbg.enterSubRule(60);

			loop60:
			while (true) {
				int alt60=2;
				try { dbg.enterDecision(60, decisionCanBacktrack[60]);

				int LA60_0 = input.LA(1);
				if ( (LA60_0==29||LA60_0==47) ) {
					alt60=1;
				}

				} finally {dbg.exitDecision(60);}

				switch (alt60) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:160:25: ( '/' | '%' ) value_expr
					{
					dbg.location(160,25);
					if ( input.LA(1)==29||input.LA(1)==47 ) {
						input.consume();
						state.errorRecovery=false;
						state.failed=false;
					}
					else {
						if (state.backtracking>0) {state.failed=true; return;}
						MismatchedSetException mse = new MismatchedSetException(null,input);
						dbg.recognitionException(mse);
						throw mse;
					}dbg.location(160,37);
					pushFollow(FOLLOW_value_expr_in_div_expr1262);
					value_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop60;
				}
			}
			} finally {dbg.exitSubRule(60);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 85, div_expr_StartIndex); }

		}
		dbg.location(160, 48);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "div_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "div_expr"



	// $ANTLR start "value_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:1: value_expr : ( bracket_expr | cast_expr | call_expr | unary_expr );
	public final void value_expr() throws  {
		int value_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "value_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(161, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 86) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:11: ( bracket_expr | cast_expr | call_expr | unary_expr )
			int alt61=4;
			try { dbg.enterDecision(61, decisionCanBacktrack[61]);

			switch ( input.LA(1) ) {
			case 33:
				{
				switch ( input.LA(2) ) {
				case 33:
				case 41:
				case 80:
				case 90:
					{
					alt61=1;
					}
					break;
				case IDENT:
					{
					int LA61_5 = input.LA(3);
					if ( (synpred89_XieXie()) ) {
						alt61=1;
					}
					else if ( (synpred90_XieXie()) ) {
						alt61=2;
					}

					else {
						if (state.backtracking>0) {state.failed=true; return;}
						int nvaeMark = input.mark();
						try {
							for (int nvaeConsume = 0; nvaeConsume < 3 - 1; nvaeConsume++) {
								input.consume();
							}
							NoViableAltException nvae =
								new NoViableAltException("", 61, 5, input);
							dbg.recognitionException(nvae);
							throw nvae;
						} finally {
							input.rewind(nvaeMark);
						}
					}

					}
					break;
				case BOOL_TYPE_DENOTER:
				case FLOAT_TYPE_DENOTER:
				case INT_TYPE_DENOTER:
					{
					alt61=2;
					}
					break;
				default:
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 61, 1, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}
				}
				break;
			case IDENT:
				{
				alt61=3;
				}
				break;
			case 41:
			case 80:
			case 90:
				{
				alt61=4;
				}
				break;
			default:
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 61, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}
			} finally {dbg.exitDecision(61);}

			switch (alt61) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:34: bracket_expr
					{
					dbg.location(161,34);
					pushFollow(FOLLOW_bracket_expr_in_value_expr1273);
					bracket_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:49: cast_expr
					{
					dbg.location(161,49);
					pushFollow(FOLLOW_cast_expr_in_value_expr1277);
					cast_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 3 :
					dbg.enterAlt(3);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:61: call_expr
					{
					dbg.location(161,61);
					pushFollow(FOLLOW_call_expr_in_value_expr1281);
					call_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 4 :
					dbg.enterAlt(4);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:73: unary_expr
					{
					dbg.location(161,73);
					pushFollow(FOLLOW_unary_expr_in_value_expr1285);
					unary_expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 86, value_expr_StartIndex); }

		}
		dbg.location(161, 82);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "value_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "value_expr"



	// $ANTLR start "bracket_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:163:1: bracket_expr : '(' arithmetic_expr ')' ;
	public final void bracket_expr() throws  {
		int bracket_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "bracket_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(163, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 87) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:163:13: ( '(' arithmetic_expr ')' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:163:16: '(' arithmetic_expr ')'
			{
			dbg.location(163,16);
			match(input,33,FOLLOW_33_in_bracket_expr1293); if (state.failed) return;dbg.location(163,20);
			pushFollow(FOLLOW_arithmetic_expr_in_bracket_expr1295);
			arithmetic_expr();
			state._fsp--;
			if (state.failed) return;dbg.location(163,36);
			match(input,34,FOLLOW_34_in_bracket_expr1297); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 87, bracket_expr_StartIndex); }

		}
		dbg.location(163, 38);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "bracket_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "bracket_expr"



	// $ANTLR start "cast_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:164:1: cast_expr : '(' type_denoter ')' value_expr ;
	public final void cast_expr() throws  {
		int cast_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "cast_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(164, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 88) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:164:10: ( '(' type_denoter ')' value_expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:164:15: '(' type_denoter ')' value_expr
			{
			dbg.location(164,15);
			match(input,33,FOLLOW_33_in_cast_expr1306); if (state.failed) return;dbg.location(164,19);
			pushFollow(FOLLOW_type_denoter_in_cast_expr1308);
			type_denoter();
			state._fsp--;
			if (state.failed) return;dbg.location(164,32);
			match(input,34,FOLLOW_34_in_cast_expr1310); if (state.failed) return;dbg.location(164,36);
			pushFollow(FOLLOW_value_expr_in_cast_expr1312);
			value_expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 88, cast_expr_StartIndex); }

		}
		dbg.location(164, 45);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "cast_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "cast_expr"



	// $ANTLR start "call_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:165:1: call_expr : function_call ;
	public final void call_expr() throws  {
		int call_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "call_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(165, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 89) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:165:10: ( function_call )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:165:14: function_call
			{
			dbg.location(165,14);
			pushFollow(FOLLOW_function_call_in_call_expr1320);
			function_call();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 89, call_expr_StartIndex); }

		}
		dbg.location(165, 26);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "call_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "call_expr"



	// $ANTLR start "unary_expr"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:166:1: unary_expr : ( '~' | '-' | 'not' ) value_expr ;
	public final void unary_expr() throws  {
		int unary_expr_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "unary_expr");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(166, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 90) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:166:11: ( ( '~' | '-' | 'not' ) value_expr )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:166:15: ( '~' | '-' | 'not' ) value_expr
			{
			dbg.location(166,15);
			if ( input.LA(1)==41||input.LA(1)==80||input.LA(1)==90 ) {
				input.consume();
				state.errorRecovery=false;
				state.failed=false;
			}
			else {
				if (state.backtracking>0) {state.failed=true; return;}
				MismatchedSetException mse = new MismatchedSetException(null,input);
				dbg.recognitionException(mse);
				throw mse;
			}dbg.location(166,35);
			pushFollow(FOLLOW_value_expr_in_unary_expr1340);
			value_expr();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 90, unary_expr_StartIndex); }

		}
		dbg.location(166, 44);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "unary_expr");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "unary_expr"



	// $ANTLR start "function_call"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:168:1: function_call : variable_ident '(' ( argument_list )? ')' ;
	public final void function_call() throws  {
		int function_call_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "function_call");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(168, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 91) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:168:14: ( variable_ident '(' ( argument_list )? ')' )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:168:16: variable_ident '(' ( argument_list )? ')'
			{
			dbg.location(168,16);
			pushFollow(FOLLOW_variable_ident_in_function_call1347);
			variable_ident();
			state._fsp--;
			if (state.failed) return;dbg.location(168,31);
			match(input,33,FOLLOW_33_in_function_call1349); if (state.failed) return;dbg.location(168,35);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:168:35: ( argument_list )?
			int alt62=2;
			try { dbg.enterSubRule(62);
			try { dbg.enterDecision(62, decisionCanBacktrack[62]);

			int LA62_0 = input.LA(1);
			if ( (LA62_0==IDENT||LA62_0==33||LA62_0==41||LA62_0==80||LA62_0==86||LA62_0==90) ) {
				alt62=1;
			}
			} finally {dbg.exitDecision(62);}

			switch (alt62) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:168:35: argument_list
					{
					dbg.location(168,35);
					pushFollow(FOLLOW_argument_list_in_function_call1351);
					argument_list();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(62);}
			dbg.location(168,50);
			match(input,34,FOLLOW_34_in_function_call1354); if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 91, function_call_StartIndex); }

		}
		dbg.location(168, 52);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "function_call");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "function_call"



	// $ANTLR start "argument_list"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:169:1: argument_list : argument ( ',' argument )* ;
	public final void argument_list() throws  {
		int argument_list_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "argument_list");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(169, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 92) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:169:14: ( argument ( ',' argument )* )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:169:16: argument ( ',' argument )*
			{
			dbg.location(169,16);
			pushFollow(FOLLOW_argument_in_argument_list1360);
			argument();
			state._fsp--;
			if (state.failed) return;dbg.location(169,25);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:169:25: ( ',' argument )*
			try { dbg.enterSubRule(63);

			loop63:
			while (true) {
				int alt63=2;
				try { dbg.enterDecision(63, decisionCanBacktrack[63]);

				int LA63_0 = input.LA(1);
				if ( (LA63_0==40) ) {
					alt63=1;
				}

				} finally {dbg.exitDecision(63);}

				switch (alt63) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:169:26: ',' argument
					{
					dbg.location(169,26);
					match(input,40,FOLLOW_40_in_argument_list1363); if (state.failed) return;dbg.location(169,30);
					pushFollow(FOLLOW_argument_in_argument_list1365);
					argument();
					state._fsp--;
					if (state.failed) return;
					}
					break;

				default :
					break loop63;
				}
			}
			} finally {dbg.exitSubRule(63);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 92, argument_list_StartIndex); }

		}
		dbg.location(169, 39);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "argument_list");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "argument_list"



	// $ANTLR start "argument"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:170:1: argument : ( expr | variable_name ':' expr ) ;
	public final void argument() throws  {
		int argument_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "argument");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(170, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 93) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:170:9: ( ( expr | variable_name ':' expr ) )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:170:12: ( expr | variable_name ':' expr )
			{
			dbg.location(170,12);
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:170:12: ( expr | variable_name ':' expr )
			int alt64=2;
			try { dbg.enterSubRule(64);
			try { dbg.enterDecision(64, decisionCanBacktrack[64]);

			int LA64_0 = input.LA(1);
			if ( (LA64_0==33||LA64_0==41||LA64_0==80||LA64_0==86||LA64_0==90) ) {
				alt64=1;
			}
			else if ( (LA64_0==IDENT) ) {
				int LA64_2 = input.LA(2);
				if ( (LA64_2==33||LA64_2==45) ) {
					alt64=1;
				}
				else if ( (LA64_2==49) ) {
					alt64=2;
				}

				else {
					if (state.backtracking>0) {state.failed=true; return;}
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 64, 2, input);
						dbg.recognitionException(nvae);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}

			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 64, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(64);}

			switch (alt64) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:170:13: expr
					{
					dbg.location(170,13);
					pushFollow(FOLLOW_expr_in_argument1375);
					expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:170:20: variable_name ':' expr
					{
					dbg.location(170,20);
					pushFollow(FOLLOW_variable_name_in_argument1379);
					variable_name();
					state._fsp--;
					if (state.failed) return;dbg.location(170,34);
					match(input,49,FOLLOW_49_in_argument1381); if (state.failed) return;dbg.location(170,38);
					pushFollow(FOLLOW_expr_in_argument1383);
					expr();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
			} finally {dbg.exitSubRule(64);}

			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 93, argument_StartIndex); }

		}
		dbg.location(170, 42);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "argument");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "argument"



	// $ANTLR start "type_denoter"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:173:1: type_denoter : ( buildin_type_denoter | custom_type_denoter );
	public final void type_denoter() throws  {
		int type_denoter_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "type_denoter");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(173, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 94) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:173:14: ( buildin_type_denoter | custom_type_denoter )
			int alt65=2;
			try { dbg.enterDecision(65, decisionCanBacktrack[65]);

			int LA65_0 = input.LA(1);
			if ( (LA65_0==BOOL_TYPE_DENOTER||LA65_0==FLOAT_TYPE_DENOTER||LA65_0==INT_TYPE_DENOTER) ) {
				alt65=1;
			}
			else if ( (LA65_0==IDENT) ) {
				alt65=2;
			}

			else {
				if (state.backtracking>0) {state.failed=true; return;}
				NoViableAltException nvae =
					new NoViableAltException("", 65, 0, input);
				dbg.recognitionException(nvae);
				throw nvae;
			}

			} finally {dbg.exitDecision(65);}

			switch (alt65) {
				case 1 :
					dbg.enterAlt(1);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:173:16: buildin_type_denoter
					{
					dbg.location(173,16);
					pushFollow(FOLLOW_buildin_type_denoter_in_type_denoter1393);
					buildin_type_denoter();
					state._fsp--;
					if (state.failed) return;
					}
					break;
				case 2 :
					dbg.enterAlt(2);

					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:174:7: custom_type_denoter
					{
					dbg.location(174,7);
					pushFollow(FOLLOW_custom_type_denoter_in_type_denoter1401);
					custom_type_denoter();
					state._fsp--;
					if (state.failed) return;
					}
					break;

			}
		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 94, type_denoter_StartIndex); }

		}
		dbg.location(174, 25);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "type_denoter");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "type_denoter"



	// $ANTLR start "buildin_type_denoter"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:176:1: buildin_type_denoter : ( BOOL_TYPE_DENOTER | INT_TYPE_DENOTER | FLOAT_TYPE_DENOTER );
	public final void buildin_type_denoter() throws  {
		int buildin_type_denoter_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "buildin_type_denoter");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(176, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 95) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:176:22: ( BOOL_TYPE_DENOTER | INT_TYPE_DENOTER | FLOAT_TYPE_DENOTER )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
			{
			dbg.location(176,22);
			if ( input.LA(1)==BOOL_TYPE_DENOTER||input.LA(1)==FLOAT_TYPE_DENOTER||input.LA(1)==INT_TYPE_DENOTER ) {
				input.consume();
				state.errorRecovery=false;
				state.failed=false;
			}
			else {
				if (state.backtracking>0) {state.failed=true; return;}
				MismatchedSetException mse = new MismatchedSetException(null,input);
				dbg.recognitionException(mse);
				throw mse;
			}
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 95, buildin_type_denoter_StartIndex); }

		}
		dbg.location(178, 26);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "buildin_type_denoter");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "buildin_type_denoter"



	// $ANTLR start "custom_type_denoter"
	// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:180:1: custom_type_denoter : variable_ident ;
	public final void custom_type_denoter() throws  {
		int custom_type_denoter_StartIndex = input.index();

		try { dbg.enterRule(getGrammarFileName(), "custom_type_denoter");
		if ( getRuleLevel()==0 ) {dbg.commence();}
		incRuleLevel();
		dbg.location(180, 0);

		try {
			if ( state.backtracking>0 && alreadyParsedRule(input, 96) ) { return; }

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:180:20: ( variable_ident )
			dbg.enterAlt(1);

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:180:22: variable_ident
			{
			dbg.location(180,22);
			pushFollow(FOLLOW_variable_ident_in_custom_type_denoter1436);
			variable_ident();
			state._fsp--;
			if (state.failed) return;
			}

		}
		catch (RecognitionException re) {
			reportError(re);
			recover(input,re);
		}
		finally {
			// do for sure before leaving
			if ( state.backtracking>0 ) { memoize(input, 96, custom_type_denoter_StartIndex); }

		}
		dbg.location(180, 35);

		}
		finally {
			dbg.exitRule(getGrammarFileName(), "custom_type_denoter");
			decRuleLevel();
			if ( getRuleLevel()==0 ) {dbg.terminate();}
		}

	}
	// $ANTLR end "custom_type_denoter"

	// $ANTLR start synpred6_XieXie
	public final void synpred6_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:23:9: ( decl_stmnt )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:23:9: decl_stmnt
		{
		dbg.location(23,9);
		pushFollow(FOLLOW_decl_stmnt_in_synpred6_XieXie94);
		decl_stmnt();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred6_XieXie

	// $ANTLR start synpred9_XieXie
	public final void synpred9_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:26:5: ( assign_stmnt )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:26:5: assign_stmnt
		{
		dbg.location(26,5);
		pushFollow(FOLLOW_assign_stmnt_in_synpred9_XieXie112);
		assign_stmnt();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred9_XieXie

	// $ANTLR start synpred10_XieXie
	public final void synpred10_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:29:14: ( variable_decl_stmnt )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:29:14: variable_decl_stmnt
		{
		dbg.location(29,14);
		pushFollow(FOLLOW_variable_decl_stmnt_in_synpred10_XieXie126);
		variable_decl_stmnt();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred10_XieXie

	// $ANTLR start synpred15_XieXie
	public final void synpred15_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:38:14: ( for_stmnt )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:38:14: for_stmnt
		{
		dbg.location(38,14);
		pushFollow(FOLLOW_for_stmnt_in_synpred15_XieXie178);
		for_stmnt();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred15_XieXie

	// $ANTLR start synpred18_XieXie
	public final void synpred18_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:41:6: ( for_range_stmnt )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:41:6: for_range_stmnt
		{
		dbg.location(41,6);
		pushFollow(FOLLOW_for_range_stmnt_in_synpred18_XieXie199);
		for_range_stmnt();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred18_XieXie

	// $ANTLR start synpred22_XieXie
	public final void synpred22_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:49:16: ( copy_assign_stmnt )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:49:16: copy_assign_stmnt
		{
		dbg.location(49,16);
		pushFollow(FOLLOW_copy_assign_stmnt_in_synpred22_XieXie247);
		copy_assign_stmnt();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred22_XieXie

	// $ANTLR start synpred23_XieXie
	public final void synpred23_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:50:7: ( modify_assign_stmnt )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:50:7: modify_assign_stmnt
		{
		dbg.location(50,7);
		pushFollow(FOLLOW_modify_assign_stmnt_in_synpred23_XieXie255);
		modify_assign_stmnt();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred23_XieXie

	// $ANTLR start synpred40_XieXie
	public final void synpred40_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:74:24: ( expr )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:74:24: expr
		{
		dbg.location(74,24);
		pushFollow(FOLLOW_expr_in_synpred40_XieXie479);
		expr();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred40_XieXie

	// $ANTLR start synpred41_XieXie
	public final void synpred41_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:77:11: ( expr_pattern )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:77:11: expr_pattern
		{
		dbg.location(77,11);
		pushFollow(FOLLOW_expr_pattern_in_synpred41_XieXie489);
		expr_pattern();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred41_XieXie

	// $ANTLR start synpred47_XieXie
	public final void synpred47_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:93:12: ( variable_decl_stmnt )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:93:12: variable_decl_stmnt
		{
		dbg.location(93,12);
		pushFollow(FOLLOW_variable_decl_stmnt_in_synpred47_XieXie631);
		variable_decl_stmnt();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred47_XieXie

	// $ANTLR start synpred51_XieXie
	public final void synpred51_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:105:33: ( '.' variable_name )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:105:33: '.' variable_name
		{
		dbg.location(105,33);
		match(input,45,FOLLOW_45_in_synpred51_XieXie730); if (state.failed) return;dbg.location(105,37);
		pushFollow(FOLLOW_variable_name_in_synpred51_XieXie732);
		variable_name();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred51_XieXie

	// $ANTLR start synpred89_XieXie
	public final void synpred89_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:34: ( bracket_expr )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:34: bracket_expr
		{
		dbg.location(161,34);
		pushFollow(FOLLOW_bracket_expr_in_synpred89_XieXie1273);
		bracket_expr();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred89_XieXie

	// $ANTLR start synpred90_XieXie
	public final void synpred90_XieXie_fragment() throws  {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:49: ( cast_expr )
		dbg.enterAlt(1);

		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:161:49: cast_expr
		{
		dbg.location(161,49);
		pushFollow(FOLLOW_cast_expr_in_synpred90_XieXie1277);
		cast_expr();
		state._fsp--;
		if (state.failed) return;
		}

	}
	// $ANTLR end synpred90_XieXie

	// Delegated rules

	public final boolean synpred18_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred18_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred9_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred9_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred40_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred40_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred47_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred47_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred41_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred41_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred51_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred51_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred10_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred10_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred6_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred6_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred90_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred90_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred15_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred15_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred23_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred23_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred89_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred89_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}
	public final boolean synpred22_XieXie() {
		state.backtracking++;
		dbg.beginBacktrack(state.backtracking);
		int start = input.mark();
		try {
			synpred22_XieXie_fragment(); // can never throw exception
		} catch (RecognitionException re) {
			System.err.println("impossible: "+re);
		}
		boolean success = !state.failed;
		input.rewind(start);
		dbg.endBacktrack(state.backtracking, success);
		state.backtracking--;
		state.failed=false;
		return success;
	}



	public static final BitSet FOLLOW_class_decl_stmnt_list_in_program43 = new BitSet(new long[]{0x0000000000000000L});
	public static final BitSet FOLLOW_EOF_in_program46 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_86_in_code_block53 = new BitSet(new long[]{0x20000000000A4080L,0x00000000023CFEB4L});
	public static final BitSet FOLLOW_stmnt_list_in_code_block55 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
	public static final BitSet FOLLOW_89_in_code_block58 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_stmnt_in_stmnt_list69 = new BitSet(new long[]{0x20000000000A4082L,0x00000000003CFEB4L});
	public static final BitSet FOLLOW_decl_stmnt_in_decl_stmnt_list78 = new BitSet(new long[]{0x20000000000A4082L,0x0000000000080E10L});
	public static final BitSet FOLLOW_class_decl_stmnt_in_class_decl_stmnt_list85 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000410L});
	public static final BitSet FOLLOW_decl_stmnt_in_stmnt94 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_branch_stmnt_in_stmnt100 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_loop_stmnt_in_stmnt106 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_assign_stmnt_in_stmnt112 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_ctrl_transfer_stmnt_in_stmnt118 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_decl_stmnt_in_decl_stmnt126 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_class_decl_stmnt_in_decl_stmnt133 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_enum_decl_stmnt_in_decl_stmnt140 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_flags_decl_stmnt_in_decl_stmnt147 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_function_decl_stmnt_in_decl_stmnt154 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_if_stmnt_in_branch_stmnt162 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_switch_stmnt_in_branch_stmnt170 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_for_stmnt_in_loop_stmnt178 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_for_each_stmnt_in_loop_stmnt185 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_for_ever_stmnt_in_loop_stmnt192 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_for_range_stmnt_in_loop_stmnt199 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_while_stmnt_in_loop_stmnt206 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_do_while_stmnt_in_loop_stmnt213 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_break_stmnt_in_ctrl_transfer_stmnt221 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_continue_stmnt_in_ctrl_transfer_stmnt230 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_return_stmnt_in_ctrl_transfer_stmnt239 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_copy_assign_stmnt_in_assign_stmnt247 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_modify_assign_stmnt_in_assign_stmnt255 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_post_operator_stmnt_in_assign_stmnt263 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_ident_in_copy_assign_stmnt272 = new BitSet(new long[]{0x0004000000000000L});
	public static final BitSet FOLLOW_50_in_copy_assign_stmnt274 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_copy_assign_stmnt276 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_ident_in_modify_assign_stmnt282 = new BitSet(new long[]{0x1041089140000000L,0x0000000001000001L});
	public static final BitSet FOLLOW_set_in_modify_assign_stmnt284 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_modify_assign_stmnt324 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_ident_in_post_operator_stmnt330 = new BitSet(new long[]{0x0000044000000000L});
	public static final BitSet FOLLOW_set_in_post_operator_stmnt332 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_79_in_if_stmnt347 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_if_condition_in_if_stmnt349 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_code_block_in_if_stmnt351 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000100L});
	public static final BitSet FOLLOW_else_stmnt_in_if_stmnt353 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_expr_in_if_condition360 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_72_in_else_stmnt367 = new BitSet(new long[]{0x0000000000000000L,0x0000000000408000L});
	public static final BitSet FOLLOW_if_stmnt_in_else_stmnt370 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_code_block_in_else_stmnt374 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_84_in_switch_stmnt383 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_switch_stmnt385 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_86_in_switch_stmnt387 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000048L});
	public static final BitSet FOLLOW_switch_case_list_in_switch_stmnt389 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
	public static final BitSet FOLLOW_89_in_switch_stmnt392 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_switch_case_in_switch_case_list398 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000048L});
	public static final BitSet FOLLOW_case_label_in_switch_case407 = new BitSet(new long[]{0x20000000000A4080L,0x00000000003CFEB4L});
	public static final BitSet FOLLOW_default_label_in_switch_case411 = new BitSet(new long[]{0x20000000000A4080L,0x00000000003CFEB4L});
	public static final BitSet FOLLOW_stmnt_list_in_switch_case414 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_67_in_case_label422 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_case_item_list_in_case_label424 = new BitSet(new long[]{0x0002000000000000L});
	public static final BitSet FOLLOW_49_in_case_label426 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_70_in_default_label433 = new BitSet(new long[]{0x0002000000000000L});
	public static final BitSet FOLLOW_49_in_default_label435 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_case_item_in_case_item_list442 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_40_in_case_item_list445 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_case_item_in_case_item_list447 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_pattern_in_case_item457 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_66_in_break_stmnt465 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_69_in_continue_stmnt471 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_82_in_return_stmnt477 = new BitSet(new long[]{0x0000020200020002L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_return_stmnt479 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_expr_pattern_in_pattern489 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_enum_case_pattern_in_pattern495 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_expr_in_expr_pattern504 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_ident_in_enum_case_pattern512 = new BitSet(new long[]{0x0000200000000000L});
	public static final BitSet FOLLOW_45_in_enum_case_pattern514 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_enum_case_name_in_enum_case_pattern518 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_IDENT_in_enum_case_name525 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_76_in_for_stmnt535 = new BitSet(new long[]{0x00080000000A4080L});
	public static final BitSet FOLLOW_for_init_in_for_stmnt537 = new BitSet(new long[]{0x0008000000000000L});
	public static final BitSet FOLLOW_51_in_for_stmnt540 = new BitSet(new long[]{0x0008020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_for_stmnt542 = new BitSet(new long[]{0x0008000000000000L});
	public static final BitSet FOLLOW_51_in_for_stmnt545 = new BitSet(new long[]{0x0000000000020000L,0x0000000000400000L});
	public static final BitSet FOLLOW_assign_stmnt_in_for_stmnt547 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_code_block_in_for_stmnt550 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_77_in_for_each_stmnt557 = new BitSet(new long[]{0x00000000000A4080L});
	public static final BitSet FOLLOW_for_each_init_in_for_each_stmnt559 = new BitSet(new long[]{0x0002000000000000L});
	public static final BitSet FOLLOW_49_in_for_each_stmnt561 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_for_each_stmnt563 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_code_block_in_for_each_stmnt565 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_78_in_for_ever_stmnt572 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_code_block_in_for_ever_stmnt574 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_76_in_for_range_stmnt580 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_name_in_for_range_stmnt582 = new BitSet(new long[]{0x0002000000000000L});
	public static final BitSet FOLLOW_49_in_for_range_stmnt584 = new BitSet(new long[]{0x0000000000040000L});
	public static final BitSet FOLLOW_for_range_in_for_range_stmnt586 = new BitSet(new long[]{0x0000400000000000L});
	public static final BitSet FOLLOW_46_in_for_range_stmnt588 = new BitSet(new long[]{0x0000000000040000L});
	public static final BitSet FOLLOW_for_range_in_for_range_stmnt590 = new BitSet(new long[]{0x0000100000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_44_in_for_range_stmnt593 = new BitSet(new long[]{0x0000000000040000L});
	public static final BitSet FOLLOW_for_range_in_for_range_stmnt595 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_code_block_in_for_range_stmnt599 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_85_in_while_stmnt606 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_while_stmnt608 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_code_block_in_while_stmnt610 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_71_in_do_while_stmnt617 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_code_block_in_do_while_stmnt619 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
	public static final BitSet FOLLOW_85_in_do_while_stmnt621 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_do_while_stmnt623 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_decl_stmnt_in_for_init631 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_assign_stmnt_in_for_init635 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_decl_stmnt_in_for_each_init641 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_INT_LITERAL_in_for_range648 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_61_in_attribute_prefix658 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_attribute_list_in_attribute_prefix660 = new BitSet(new long[]{0x4000000000000000L});
	public static final BitSet FOLLOW_62_in_attribute_prefix662 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_attribute_in_attribute_list671 = new BitSet(new long[]{0x0000010000000000L});
	public static final BitSet FOLLOW_40_in_attribute_list674 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_attribute_in_attribute_list676 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_IDENT_in_attribute688 = new BitSet(new long[]{0x0000000200000002L});
	public static final BitSet FOLLOW_33_in_attribute691 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_attribute_argument_list_in_attribute693 = new BitSet(new long[]{0x0000000400000000L});
	public static final BitSet FOLLOW_34_in_attribute695 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_attribute_argument_in_attribute_argument_list703 = new BitSet(new long[]{0x0000010000000000L});
	public static final BitSet FOLLOW_40_in_attribute_argument_list706 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_attribute_argument_in_attribute_argument_list708 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_expr_in_attribute_argument718 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_name_in_variable_ident727 = new BitSet(new long[]{0x0000200000000002L});
	public static final BitSet FOLLOW_45_in_variable_ident730 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_name_in_variable_ident732 = new BitSet(new long[]{0x0000200000000002L});
	public static final BitSet FOLLOW_IDENT_in_variable_name741 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_type_denoter_in_variable_decl_stmnt748 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_IDENT_in_variable_decl_stmnt750 = new BitSet(new long[]{0x0004000000000002L});
	public static final BitSet FOLLOW_variable_init_in_variable_decl_stmnt752 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_50_in_variable_init761 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_variable_init763 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_class_modifier_in_class_decl_stmnt771 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000010L});
	public static final BitSet FOLLOW_68_in_class_decl_stmnt774 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_class_name_in_class_decl_stmnt776 = new BitSet(new long[]{0x0002000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_type_inheritance_in_class_decl_stmnt778 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_class_body_in_class_decl_stmnt781 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_74_in_class_modifier788 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_86_in_class_body796 = new BitSet(new long[]{0x20000000000A4080L,0x0000000002080E10L});
	public static final BitSet FOLLOW_decl_stmnt_list_in_class_body798 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
	public static final BitSet FOLLOW_89_in_class_body801 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_IDENT_in_class_name809 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_49_in_type_inheritance817 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_ident_in_type_inheritance819 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_49_in_type_multi_inheritance825 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_ident_in_type_multi_inheritance827 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_40_in_type_multi_inheritance830 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_ident_in_type_multi_inheritance832 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_73_in_enum_decl_stmnt842 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_name_in_enum_decl_stmnt844 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_enum_body_in_enum_decl_stmnt846 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_86_in_enum_body854 = new BitSet(new long[]{0x0000000000020000L,0x0000000002000000L});
	public static final BitSet FOLLOW_enum_entry_list_in_enum_body856 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
	public static final BitSet FOLLOW_89_in_enum_body859 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_enum_entry_in_enum_entry_list865 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_40_in_enum_entry_list868 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_enum_entry_in_enum_entry_list870 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_enum_entry_name_in_enum_entry880 = new BitSet(new long[]{0x0004000000000002L});
	public static final BitSet FOLLOW_50_in_enum_entry883 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_enum_entry885 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_IDENT_in_enum_entry_name893 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_75_in_flags_decl_stmnt901 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_name_in_flags_decl_stmnt903 = new BitSet(new long[]{0x0002000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_type_multi_inheritance_in_flags_decl_stmnt905 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_flags_body_in_flags_decl_stmnt908 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_86_in_flags_body916 = new BitSet(new long[]{0x0000000000020000L,0x0000000002000000L});
	public static final BitSet FOLLOW_flags_entry_list_in_flags_body918 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
	public static final BitSet FOLLOW_89_in_flags_body921 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_flags_entry_in_flags_entry_list927 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_40_in_flags_entry_list930 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_flags_entry_in_flags_entry_list932 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_IDENT_in_flags_entry941 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_attribute_prefix_in_function_decl_stmnt949 = new BitSet(new long[]{0x00000000000A4080L,0x0000000000080000L});
	public static final BitSet FOLLOW_function_head_in_function_decl_stmnt952 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
	public static final BitSet FOLLOW_code_block_in_function_decl_stmnt954 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_function_modifier_in_function_head962 = new BitSet(new long[]{0x00000000000A4080L});
	public static final BitSet FOLLOW_type_denoter_in_function_head965 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_name_in_function_head967 = new BitSet(new long[]{0x0000000200000000L});
	public static final BitSet FOLLOW_33_in_function_head969 = new BitSet(new long[]{0x00000004000A4080L});
	public static final BitSet FOLLOW_parameter_list_in_function_head971 = new BitSet(new long[]{0x0000000400000000L});
	public static final BitSet FOLLOW_34_in_function_head974 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_83_in_function_modifier981 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_parameter_in_parameter_list989 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_40_in_parameter_list992 = new BitSet(new long[]{0x00000000000A4080L});
	public static final BitSet FOLLOW_parameter_in_parameter_list994 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_type_denoter_in_parameter1005 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_name_in_parameter1007 = new BitSet(new long[]{0x0004000000000002L});
	public static final BitSet FOLLOW_50_in_parameter1010 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_parameter1012 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_expr_in_expr_list1022 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_40_in_expr_list1025 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_expr_list1027 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_arithmetic_expr_in_expr1037 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_init_list_expr_in_expr1043 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_86_in_init_list_expr1050 = new BitSet(new long[]{0x0000020200020000L,0x0000000006410000L});
	public static final BitSet FOLLOW_expr_list_in_init_list_expr1052 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
	public static final BitSet FOLLOW_89_in_init_list_expr1055 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_logic_or_expr_in_arithmetic_expr1062 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_logic_and_expr_in_logic_or_expr1069 = new BitSet(new long[]{0x0000000000000002L,0x0000000000020000L});
	public static final BitSet FOLLOW_81_in_logic_or_expr1072 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_logic_and_expr_in_logic_or_expr1074 = new BitSet(new long[]{0x0000000000000002L,0x0000000000020000L});
	public static final BitSet FOLLOW_bitwise_or_expr_in_logic_and_expr1083 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000002L});
	public static final BitSet FOLLOW_65_in_logic_and_expr1086 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_bitwise_or_expr_in_logic_and_expr1088 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000002L});
	public static final BitSet FOLLOW_bitwise_xor_expr_in_bitwise_or_expr1096 = new BitSet(new long[]{0x0000000000000002L,0x0000000000800000L});
	public static final BitSet FOLLOW_87_in_bitwise_or_expr1099 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_bitwise_xor_expr_in_bitwise_or_expr1101 = new BitSet(new long[]{0x0000000000000002L,0x0000000000800000L});
	public static final BitSet FOLLOW_bitwise_and_expr_in_bitwise_xor_expr1109 = new BitSet(new long[]{0x8000000000000002L});
	public static final BitSet FOLLOW_63_in_bitwise_xor_expr1112 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_bitwise_and_expr_in_bitwise_xor_expr1114 = new BitSet(new long[]{0x8000000000000002L});
	public static final BitSet FOLLOW_equality_expr_in_bitwise_and_expr1122 = new BitSet(new long[]{0x0000000080000002L});
	public static final BitSet FOLLOW_31_in_bitwise_and_expr1125 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_equality_expr_in_bitwise_and_expr1127 = new BitSet(new long[]{0x0000000080000002L});
	public static final BitSet FOLLOW_relation_expr_in_equality_expr1136 = new BitSet(new long[]{0x0100000010000002L});
	public static final BitSet FOLLOW_set_in_equality_expr1139 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_relation_expr_in_equality_expr1147 = new BitSet(new long[]{0x0100000010000002L});
	public static final BitSet FOLLOW_shift_expr_in_relation_expr1156 = new BitSet(new long[]{0x0690000000000002L});
	public static final BitSet FOLLOW_set_in_relation_expr1159 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_shift_expr_in_relation_expr1175 = new BitSet(new long[]{0x0690000000000002L});
	public static final BitSet FOLLOW_add_expr_in_shift_expr1185 = new BitSet(new long[]{0x0820000000000002L});
	public static final BitSet FOLLOW_set_in_shift_expr1188 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_add_expr_in_shift_expr1196 = new BitSet(new long[]{0x0820000000000002L});
	public static final BitSet FOLLOW_sub_expr_in_add_expr1206 = new BitSet(new long[]{0x0000002000000002L});
	public static final BitSet FOLLOW_37_in_add_expr1209 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_sub_expr_in_add_expr1211 = new BitSet(new long[]{0x0000002000000002L});
	public static final BitSet FOLLOW_mul_expr_in_sub_expr1221 = new BitSet(new long[]{0x0000020000000002L});
	public static final BitSet FOLLOW_41_in_sub_expr1224 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_mul_expr_in_sub_expr1226 = new BitSet(new long[]{0x0000020000000002L});
	public static final BitSet FOLLOW_div_expr_in_mul_expr1236 = new BitSet(new long[]{0x0000000800000002L});
	public static final BitSet FOLLOW_35_in_mul_expr1239 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_div_expr_in_mul_expr1241 = new BitSet(new long[]{0x0000000800000002L});
	public static final BitSet FOLLOW_value_expr_in_div_expr1251 = new BitSet(new long[]{0x0000800020000002L});
	public static final BitSet FOLLOW_set_in_div_expr1254 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_value_expr_in_div_expr1262 = new BitSet(new long[]{0x0000800020000002L});
	public static final BitSet FOLLOW_bracket_expr_in_value_expr1273 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_cast_expr_in_value_expr1277 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_call_expr_in_value_expr1281 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_unary_expr_in_value_expr1285 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_33_in_bracket_expr1293 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_arithmetic_expr_in_bracket_expr1295 = new BitSet(new long[]{0x0000000400000000L});
	public static final BitSet FOLLOW_34_in_bracket_expr1297 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_33_in_cast_expr1306 = new BitSet(new long[]{0x00000000000A4080L});
	public static final BitSet FOLLOW_type_denoter_in_cast_expr1308 = new BitSet(new long[]{0x0000000400000000L});
	public static final BitSet FOLLOW_34_in_cast_expr1310 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_value_expr_in_cast_expr1312 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_function_call_in_call_expr1320 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_set_in_unary_expr1328 = new BitSet(new long[]{0x0000020200020000L,0x0000000004010000L});
	public static final BitSet FOLLOW_value_expr_in_unary_expr1340 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_ident_in_function_call1347 = new BitSet(new long[]{0x0000000200000000L});
	public static final BitSet FOLLOW_33_in_function_call1349 = new BitSet(new long[]{0x0000020600020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_argument_list_in_function_call1351 = new BitSet(new long[]{0x0000000400000000L});
	public static final BitSet FOLLOW_34_in_function_call1354 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_argument_in_argument_list1360 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_40_in_argument_list1363 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_argument_in_argument_list1365 = new BitSet(new long[]{0x0000010000000002L});
	public static final BitSet FOLLOW_expr_in_argument1375 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_name_in_argument1379 = new BitSet(new long[]{0x0002000000000000L});
	public static final BitSet FOLLOW_49_in_argument1381 = new BitSet(new long[]{0x0000020200020000L,0x0000000004410000L});
	public static final BitSet FOLLOW_expr_in_argument1383 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_buildin_type_denoter_in_type_denoter1393 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_custom_type_denoter_in_type_denoter1401 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_ident_in_custom_type_denoter1436 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_decl_stmnt_in_synpred6_XieXie94 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_assign_stmnt_in_synpred9_XieXie112 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_decl_stmnt_in_synpred10_XieXie126 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_for_stmnt_in_synpred15_XieXie178 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_for_range_stmnt_in_synpred18_XieXie199 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_copy_assign_stmnt_in_synpred22_XieXie247 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_modify_assign_stmnt_in_synpred23_XieXie255 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_expr_in_synpred40_XieXie479 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_expr_pattern_in_synpred41_XieXie489 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_variable_decl_stmnt_in_synpred47_XieXie631 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_45_in_synpred51_XieXie730 = new BitSet(new long[]{0x0000000000020000L});
	public static final BitSet FOLLOW_variable_name_in_synpred51_XieXie732 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_bracket_expr_in_synpred89_XieXie1273 = new BitSet(new long[]{0x0000000000000002L});
	public static final BitSet FOLLOW_cast_expr_in_synpred90_XieXie1277 = new BitSet(new long[]{0x0000000000000002L});
}
