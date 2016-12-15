/*
 * TransformCommRsb.h
 *
 *  Created on: Dec 15, 2014
 *      Author: leon
 */

#pragma once

#include <rct/impl/TransformCommunicator.h>
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#include <rsb/Listener.h>
#include <rsb/Informer.h>
#include <boost/shared_ptr.hpp>
#include <rsc/logging/Logger.h>

namespace rct {

class TransformCommRsb: public TransformCommunicator {
public:
	typedef boost::shared_ptr<TransformCommRsb> Ptr;
	TransformCommRsb(const std::string &authority, bool legacyMode=false);
	TransformCommRsb(const std::string &authority, const TransformListener::Ptr& listener, bool legacyMode=false);
	TransformCommRsb(const std::string &authority,
			const std::vector<TransformListener::Ptr>& listeners, bool legacyMode=false);
	TransformCommRsb(const std::string &authority, const TransformListener::Ptr& listener,
			std::string scopeSync, std::string scopeTransforms, std::string scopeSuffixStatic,
			std::string scopeSuffixDynamic, std::string userKeyAuthority, bool legacyMode=false);
	TransformCommRsb(const std::string &authority,
			const std::vector<TransformListener::Ptr>& listeners, std::string scopeSync,
			std::string scopeTransforms, std::string scopeSuffixStatic,
			std::string scopeSuffixDynamic, std::string userKeyAuthority, bool legacyMode=false);
	virtual ~TransformCommRsb();

	virtual void init(const TransformerConfig &conf);
	virtual void shutdown();
	virtual void requestSync();

	/** \brief Add transform information to the rct data structure
	 * \param transform The transform to store
	 * \param authority The source of the information for this transform
	 * \param is_static Record this transform as a static transform.  It will be good across all time.  (This cannot be changed after the first call.)
	 * \return True unless an error occured
	 */
	virtual bool sendTransform(const Transform& transform);
	virtual bool sendTransform(const std::vector<Transform>& transforms);

	virtual void addTransformListener(const TransformListener::Ptr& listener);
	virtual void addTransformListener(const std::vector<TransformListener::Ptr>& listeners);
	virtual void removeTransformListener(const TransformListener::Ptr& listener);

	void printContents(std::ostream& stream) const;

	virtual std::string getAuthorityName() const;

private:
	rsb::ListenerPtr rsbListenerTransform;
	rsb::Informer<Transform>::Ptr rsbInformerTransform;
	rsb::Informer< std::vector<Transform> >::Ptr rsbInformerTransformCollection;
	rsb::ListenerPtr rsbListenerSync;
	rsb::Informer<void>::Ptr rsbInformerSync;
	std::vector<TransformListener::Ptr> listeners;
	boost::mutex mutex_listener, mutex_staticCache, mutex_dynamicCache;
	std::map< std::string, std::map<std::string, Transform> > sendCacheDynamic;
	std::map< std::string, std::map<std::string, Transform> > sendCacheStatic;
	std::string authority;
	rsb::HandlerPtr transformHandler;
	rsb::HandlerPtr syncHandler;
	std::string scopeSync;
	std::string scopeTransforms;
	std::string scopeSuffixStatic;
	std::string scopeSuffixDynamic;
	std::string userKeyAuthority;
	bool legacyMode;


	void transformCallback(rsb::EventPtr t);
	void triggerCallback(rsb::EventPtr t);
	void publishCache();

	static std::string defaultScopeSync;
	static std::string defaultScopeTransforms;
	static std::string defaultScopeSufficStatic;
	static std::string defaultScopeSuffixDynamic;
	static std::string defaultUserKeyAuthority;
	static rsc::logging::LoggerPtr logger;
};
}  // namespace rct
