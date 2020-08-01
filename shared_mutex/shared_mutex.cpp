//template <typename... TArgs> void joinAll(TArgs&&... args) 
//{ 
//	static_assert(
//		(      std::is_base_of_v<std::thread, std::decay_t<TArgs>>&&...                ), "스레드만 받아요..."); 
//
//	(args.join(), ...); 
//}

template<typename ...t>
void q(t&&... s)
{
	static_assert(     (t&&...),"q" );
}

int main()
{
	q(1, 2, 3, 4, 5, 6, 7);
}