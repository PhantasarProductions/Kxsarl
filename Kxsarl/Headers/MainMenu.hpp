namespace Kxsarl {

	class MainMenuItem;

	class MainMenuItem {
	private:
		MainMenuItem(std::string _Caption, void (*_Action)(MainMenuItem*) = nullptr, bool (*_Allow)(MainMenuItem*) = nullptr);
	public:
		std::string Caption{ "" };
		void (*Action)(MainMenuItem*) { nullptr };
		bool (*Allow)(MainMenuItem*) { nullptr };

		static void InitMenuItems();
	};

}