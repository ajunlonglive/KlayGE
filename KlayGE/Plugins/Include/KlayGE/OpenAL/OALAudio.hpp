// OALAudio.hpp
// KlayGE OpenAL声音引擎 头文件
// Ver 3.2.0
// 版权所有(C) 龚敏敏, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// 改进了OALMusicBuffer中线程的使用 (2006.4.29)
//
// 2.0.0
// 初次建立 (2003.7.7)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OALAUDIO_HPP
#define _OALAUDIO_HPP

#include <KlayGE/PreDeclare.hpp>

#include <al/al.h>
#include <al/alc.h>

#include <vector>
#define NOMINMAX
#include <windows.h>

#pragma warning(disable: 4251 4275)
#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include <KlayGE/Audio.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_AudioEngine_OpenAL_d.lib")
#else
	#pragma comment(lib, "KlayGE_AudioEngine_OpenAL.lib")
#endif

namespace KlayGE
{
	ALenum Convert(AudioFormat format);
	float3 VecToALVec(float3 const & v);
	float3 ALVecToVec(float3 const & v);

	// 声音缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	class OALSoundBuffer : boost::noncopyable, public SoundBuffer
	{
	private:
		typedef std::vector<ALuint>				Sources_type;
		typedef Sources_type::iterator			SourcesIter;
		typedef Sources_type::const_iterator	SourcesConstIter;

	public:
		OALSoundBuffer(AudioDataSourcePtr const & dataSource, uint32_t numSource, float volume);
		~OALSoundBuffer();

		void Play(bool loop = false);
		void Stop();

		void Volume(float vol);

		bool IsPlaying() const;

		float3 Position() const;
		void Position(float3 const & v);
		float3 Velocity() const;
		void Velocity(float3 const & v);
		float3 Direction() const;
		void Direction(float3 const & v);

	private:
		void DoReset();
		SourcesIter FreeSource();

	private:
		Sources_type	sources_;
		ALuint			buffer_;

		float3		pos_;
		float3		vel_;
		float3		dir_;
	};

	// 音乐缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	class OALMusicBuffer : boost::noncopyable, public MusicBuffer
	{
	private:
		typedef std::vector<ALuint>		Buffers;
		typedef Buffers::iterator		BuffersIter;
		typedef Buffers::const_iterator	BuffersConstIter;

	public:
		OALMusicBuffer(AudioDataSourcePtr const & dataSource, uint32_t bufferSeconds, float volume);
		~OALMusicBuffer();

		void Volume(float vol);

		bool IsPlaying() const;

		float3 Position() const;
		void Position(float3 const & v);
		float3 Velocity() const;
		void Velocity(float3 const & v);
		float3 Direction() const;
		void Direction(float3 const & v);

		void LoopUpdateBuffer();

	private:
		void DoReset();
		void DoPlay(bool loop);
		void DoStop();

	private:
		ALuint		source_;
		Buffers		bufferQueue_;

		bool		loop_;

		bool stopped_;
		boost::condition play_cond_;
		boost::mutex play_mutex_;
		boost::thread play_thread_;
	};

	// 管理音频播放
	/////////////////////////////////////////////////////////////////////////////////
	class OALAudioEngine : boost::noncopyable, public AudioEngine
	{
	public:
		OALAudioEngine();
		~OALAudioEngine();

		std::wstring const & Name() const;

		float3 GetListenerPos() const;
		void SetListenerPos(float3 const & v);
		float3 GetListenerVel() const;
		void SetListenerVel(float3 const & v);
		void GetListenerOri(float3& face, float3& up) const;
		void SetListenerOri(float3 const & face, float3 const & up);
	};
}

#endif		// _OALAUDIO_HPP
