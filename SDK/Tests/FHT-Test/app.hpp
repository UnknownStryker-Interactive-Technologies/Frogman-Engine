FE_WORLD_TAG
{
	Boot = 0,
	MainMenu = 1,
	OverWorld = 2,
	BattleArena = 3,
	LoadingScreen = 4
};


namespace Game
{
	// ------------------------------------------------------------------ 초기화 (0 ~ 3)

	FE_SYSTEM(FE::_EngineInitialization, FEWorldTag::Boot);
	inline void boot_memory_arenas(::FE::world& world_p) noexcept
	{

	}

	FE_SYSTEM(FE::_GameInstanceInitialization, FEWorldTag::Boot);
	inline void load_player_profile(::FE::world& world_p)
	{

	}

	FE_SYSTEM(FE::_WorldInitialization, FEWorldTag::OverWorld);
	void build_navmesh(::FE::world& world_p) noexcept
	{

	}

	FE_SYSTEM(FE::_WorldDefaultEntityInitialization, FEWorldTag::OverWorld);
	void spawn_persistent_actors(::FE::world& world_p)
	{

	}


	// ------------------------------------------------------------------ 시작 (4, 5)

	FE_SYSTEM(FE::_GameInstanceBegin, FEWorldTag::MainMenu);
	void play_intro_cinematic(::FE::world& world_p);

	FE_SYSTEM(FE::_WorldBegin, FEWorldTag::MainMenu);
	void fade_in_title_ui(::FE::world& world_p);

	// 같은 phase, 다른 world.
	FE_SYSTEM(FE::_WorldBegin, FEWorldTag::BattleArena);
	void seed_encounter_rng(::FE::world& world_p);


	// ------------------------------------------------------------------ 게임 인스턴스 틱 (7 ~ 9)

	FE_SYSTEM(FE::_PreGameInstanceTick, FEWorldTag::OverWorld);
	void poll_input_devices(::FE::world& world_p);

	FE_SYSTEM(FE::_GameInstanceTick, FEWorldTag::OverWorld);
	void advance_save_timer(::FE::world& world_p);

	FE_SYSTEM(FE::_PostGameInstanceTick, FEWorldTag::OverWorld);
	void flush_input_edge_flags(::FE::world& world_p);


	// ------------------------------------------------------------------ 월드 틱 (10 ~ 12)

	FE_SYSTEM(FE::_PreWorldTick, FEWorldTag::OverWorld);
	void stream_in_nearby_cells(::FE::world& world_p);

	// 같은 phase 에 서로 다른 world 세 개 — 디스패치 테이블 버킷 확인용.
	FE_SYSTEM(FE::_WorldTick, FEWorldTag::OverWorld);
	void tick_weather(::FE::world& world_p);

	FE_SYSTEM(FE::_WorldTick, FEWorldTag::BattleArena);
	void tick_arena_hazards(::FE::world& world_p);

	FE_SYSTEM(FE::_WorldTick, EditorWorldTag::Viewport);
	void tick_editor_gizmos(::FE::world& world_p);

	FE_SYSTEM(FE::_PostWorldTick, FEWorldTag::OverWorld);
	void evict_far_cells(::FE::world& world_p);


	// ------------------------------------------------------------------ 엔티티 틱 (13 ~ 15)

		// 같은 phase + 같은 world 에 함수 두 개 — 등록 순서/충돌 처리 확인용.
	FE_SYSTEM(FE::_PreEntityTick, FEWorldTag::OverWorld);
	void refresh_transform_cache(::FE::world& world_p);

	FE_SYSTEM(FE::_PreEntityTick, FEWorldTag::OverWorld);
	void rebuild_spatial_hash(::FE::world& world_p);

	// 한정 이름 형태로도 넘겨본다.
	FE_SYSTEM(FE::SystemCallPhase::_EntityTick, FEWorldTag::OverWorld);
	void run_behaviour_trees(::FE::world& world_p);

	FE_SYSTEM(FE::_PostEntityTick, FEWorldTag::OverWorld);
	void apply_deferred_despawns(::FE::world& world_p);


	// ------------------------------------------------------------------ 물리 (16 ~ 20)

	FE_SYSTEM(FE::_PrePhysics, FEWorldTag::BattleArena);
	void sync_kinematic_bodies(::FE::world& world_p);

	FE_SYSTEM(FE::_StartPhysics, FEWorldTag::BattleArena);
	void begin_broadphase(::FE::world& world_p);

	// cv 한정 상수를 SysCallPhase 로 전달.
	FE_SYSTEM(g_physics_phase, FEWorldTag::BattleArena);
	void integrate_rigid_bodies(::FE::world& world_p);

	FE_SYSTEM(FE::_EndPhysics, FEWorldTag::BattleArena);
	void resolve_contact_events(::FE::world& world_p);

	FE_SYSTEM(FE::_PostPhysics, FEWorldTag::BattleArena);
	void write_back_transforms(::FE::world& world_p);


	// ------------------------------------------------------------------ 후처리 / 렌더 (21 ~ 23)

	FE_SYSTEM(FE::_PostUpdateWork, FEWorldTag::OverWorld);
	void collect_profiling_counters(::FE::world& world_p);

	FE_SYSTEM(FE::_PreRenderQueueCommit, FEWorldTag::OverWorld);
	void cull_and_sort_draw_calls(::FE::world& world_p);

	FE_SYSTEM(FE::_PostRenderQueueCommit, FEWorldTag::OverWorld);
	void submit_debug_lines(::FE::world& world_p);


	// ------------------------------------------------------------------ 종료 (25 ~ 30)

	FE_SYSTEM(FE::_WorldEnd, FEWorldTag::BattleArena);
	void tally_battle_results(::FE::world& world_p);

	FE_SYSTEM(FE::_GameInstanceEnd, FEWorldTag::MainMenu);
	void write_session_log(::FE::world& world_p);

	FE_SYSTEM(FE::_WorldDefaultEntityDeinitialization, FEWorldTag::OverWorld);
	void destroy_persistent_actors(::FE::world& world_p);

	FE_SYSTEM(FE::_WorldDeinitialization, FEWorldTag::OverWorld);
	void release_navmesh(::FE::world& world_p);

	FE_SYSTEM(FE::_GameInstanceDeinitialization, FEWorldTag::Boot);
	void save_player_profile(::FE::world& world_p);
		
	FE_SYSTEM(FE::_EngineDeinitialization, FEWorldTag::Boot);
	void teardown_memory_arenas(::FE::world& world_p);


	// ------------------------------------------------------------------ static 멤버 함수 형태

	class CombatDirector
	{
	public: // FE_SYSTEM has no effect, FHT silently ignores class static member functions.
		FE_SYSTEM(FE::_WorldBegin, FEWorldTag::BattleArena);
		static void on_arena_opened(::FE::world& world_p);

		FE_SYSTEM(FE::_PreEntityTick, FEWorldTag::BattleArena);
		static void select_targets(::FE::world& world_p);

		FE_SYSTEM(FE::_EntityTick, FEWorldTag::BattleArena);
		static void drive_ai_state_machines(::FE::world& world_p);

		FE_SYSTEM(FE::_WorldEnd, FEWorldTag::BattleArena);
		static void on_arena_closed(::FE::world& world_p);

	private:
		FE_SYSTEM(FE::_PostEntityTick, FEWorldTag::BattleArena);
		static void prune_dead_targets(::FE::world& world_p);
	};


	class LoadingScreenDriver
	{
	public: // FE_SYSTEM has no effect, FHT silently ignores class static member functions.
		FE_SYSTEM(FE::_WorldInitialization, FEWorldTag::LoadingScreen);
		static void mount_asset_bundles(::FE::world& world_p);

		FE_SYSTEM(FE::_WorldTick, FEWorldTag::LoadingScreen);
		static void advance_progress_bar(::FE::world& world_p);

		FE_SYSTEM(FE::_WorldDeinitialization, FEWorldTag::LoadingScreen);
		static void unmount_asset_bundles(::FE::world& world_p);
	};


	class EditorInspector
	{
	public: // FE_SYSTEM has no effect, FHT silently ignores class static member functions.
		FE_SYSTEM(FE::_PostUpdateWork, EditorWorldTag::AssetBrowser);
		static void refresh_thumbnail_queue(::FE::world& world_p);

		FE_SYSTEM(FE::_PreRenderQueueCommit, EditorWorldTag::AssetBrowser);
		static void emit_selection_outline(::FE::world& world_p);
	};
}