/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	buildingEffectIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CclusterIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// buildingEffectIGC.cpp : Implementation of buildingEffectIGC

#ifndef __BUILDINGEFFECTIGC_H_
#define __BUILDINGEFFECTIGC_H_

const float c_dtShrink   = 10.0f;
const float c_dtOpaque   = 10.0f;
const float c_dtGrow     = 10.0f;
const float c_dtEnvelope = 10.0f;

class CbuildingEffectIGC : public TmodelIGC<IbuildingEffectIGC>
{
    public:
        CbuildingEffectIGC(void)
        {
        }
        ~CbuildingEffectIGC(void)
        {
        }

    public:
    // IbaseIGC
	    virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
        {
            TmodelIGC<IbuildingEffectIGC>::Initialize(pMission, now, data, dataSize);

            {
                ZRetailAssert (data && (dataSize > sizeof(DataBuildingEffectBase)));
                DataBuildingEffectBase*   dataBuilding =  (DataBuildingEffectBase*)data;

                m_positionStart = dataBuilding->positionStart;
                m_positionStop = dataBuilding->positionStop;

                m_radiusAsteroid = dataBuilding->radiusAsteroid;
                m_radiusStation = dataBuilding->radiusStation;

                m_timeEnvelope = pMission->GetIgcSite()->ClientTimeFromServerTime(dataBuilding->timeStart);
#ifdef WIN
                m_timeGrow     = m_timeEnvelope + c_dtEnvelope;
                m_timeOpaque   = m_timeGrow     + c_dtGrow;
                m_timeShrink   = m_timeOpaque   + c_dtOpaque;
                m_timeComplete = m_timeShrink   + c_dtShrink;
#else
                m_timeGrow     = m_timeEnvelope + Duration(c_dtEnvelope);
                m_timeOpaque   = m_timeGrow     + Duration(c_dtGrow);
                m_timeShrink   = m_timeOpaque   + Duration(c_dtOpaque);
                m_timeComplete = m_timeShrink   + Duration(c_dtShrink);
#endif
            }

            assert (sizeof(DataBuildingEffectIGC) != sizeof(DataBuildingEffectExport));
            IclusterIGC*    pcluster;
            const Color*    pcolor;
            if (dataSize == sizeof(DataBuildingEffectIGC))
            {
                DataBuildingEffectIGC*  dataBuilding = (DataBuildingEffectIGC*)data;
#ifdef WIN
                m_pshipBuilder = dataBuilding->pshipBuilder;
#else
                m_pshipBuilder.reset( dataBuilding->pshipBuilder );
#endif
                assert (m_pshipBuilder);
                m_pshipBuilder->SetStateM(drillingMaskIGC | buildingMaskIGC);
#ifdef WIN
                m_pside = m_pshipBuilder->GetSide();
#else
                m_pside.reset(m_pshipBuilder->GetSide());
#endif
#ifdef WIN
                m_pstationType = (IstationTypeIGC*)(m_pshipBuilder->GetBaseData());
#else
                m_pstationType.reset( (IstationTypeIGC*)m_pshipBuilder->GetBaseData() );
#endif

#ifdef WIN
                m_pasteroid = dataBuilding->pasteroid;
#else
                m_pasteroid.reset( dataBuilding->pasteroid );
#endif
                assert (m_pasteroid);
                m_pasteroid->SetBuildingEffect(this);

                pcluster = dataBuilding->pcluster;

                pcolor = &(m_pshipBuilder->GetSide()->GetColor());
            }
            else
            {
                ZRetailAssert (dataSize == sizeof(DataBuildingEffectExport));
                DataBuildingEffectExport*  dataBuilding = (DataBuildingEffectExport*)data;

                pcluster = pMission->GetCluster(dataBuilding->clusterID);
#ifdef WIN
                m_pasteroid = pcluster->GetAsteroid(dataBuilding->asteroidID);
#else
                m_pasteroid.reset( pcluster->GetAsteroid(dataBuilding->asteroidID) );
#endif
                assert (m_pasteroid);
                m_pasteroid->SetBuildingEffect(this);

                pcolor = &(dataBuilding->color);
            }

            LoadEffect(Color(50.0f/255.0f, 28.0f/255.0f, 146.0f/255.0f), //*pcolor,
                       (c_mtNotPickable | c_mtCastRay | c_mtHitable | c_mtDamagable | c_mtPredictable));

            m_radiusMax = 1.1f * (m_radiusAsteroid > m_radiusStation ? m_radiusAsteroid : m_radiusStation);
            SetRadius(1.0f);
            SetPosition(m_positionStart);

            GetHitTest()->SetNoHit(m_pasteroid->GetHitTest());

            assert (GetVelocity().LengthSquared() == 0.0f);

            assert (pcluster);
            SetCluster(pcluster);

            //Set the BB's effect, since it was probably created as a result of a collision.
            SetBB(now, now, 0.0f);

            return S_OK;
        }
	    virtual void    Terminate(void)
        {
            AddRef();

            if (m_pasteroid)
            {
                m_pasteroid->SetBuildingEffect(NULL);
                m_pasteroid = NULL;
            }

            if (m_pshipBuilder)
            {
#ifdef WIN
                GetMyMission()->GetIgcSite()->KillShipEvent(GetMyLastUpdate(), m_pshipBuilder, NULL, 0.0f, m_pshipBuilder->GetPosition(), Vector::GetZero());
#else
                GetMyMission()->GetIgcSite()->KillShipEvent(GetMyLastUpdate(), m_pshipBuilder.get(), NULL, 0.0f, m_pshipBuilder->GetPosition(), Vector::GetZero());
#endif
                m_pshipBuilder = NULL;
            }

            m_pside = NULL;
            m_pstationType = NULL;

	        TmodelIGC<IbuildingEffectIGC>::Terminate();
            Release();
        }

        int        Export(void*  data) const
        {
            if (data)
            {
                DataBuildingEffectExport*   pdbe = (DataBuildingEffectExport*)data;

                pdbe->timeStart = GetMyMission()->GetIgcSite()->ServerTimeFromClientTime(m_timeEnvelope);

                pdbe->asteroidID = m_pasteroid ? m_pasteroid->GetObjectID() : NA;
                pdbe->clusterID = GetCluster()->GetObjectID();

                pdbe->radiusAsteroid = m_radiusAsteroid;
                pdbe->radiusStation = m_radiusStation;

                pdbe->positionStart = m_positionStart;
                pdbe->positionStop = m_positionStop;

                assert (m_pshipBuilder);
                pdbe->color = m_pshipBuilder->GetSide()->GetColor();
            }

            return sizeof(DataBuildingEffectExport);
        }

        virtual void        Update(Time now)
        {
            if (now >= m_timeComplete)
            {
                Terminate();
            }
            else
            {
                Vector  position;
                float   radius;
                float   aInner;
                float   fInner;
                float   fOuter;

                if (now >= m_timeShrink)
                {
#ifdef WIN
                    float   f = (now - m_timeShrink) / c_dtShrink;
#else
                    float f = (now - m_timeShrink).count() / c_dtShrink;
#endif
                    position = m_positionStop;
                    radius = m_radiusStation * f + m_radiusMax * (1.0f - f);

                    aInner = 1.0f - f;
                    fInner = 1.0f - f;
                    fOuter = 1.0f - f;
                }
                else if (now >= m_timeOpaque)
                {
#ifdef WIN
                    float   f = (now - m_timeOpaque) / c_dtOpaque;
#else
                    float f = (now - m_timeOpaque).count() / c_dtOpaque;
#endif

                    position = m_positionStop;
                    radius = m_radiusMax;

                    aInner = 0.5f + 0.5f * f;
                    fInner = 1.0f;
                    fOuter = 1.0f;
                }
                else if (now >= m_timeGrow)
                {
#ifdef WIN
                    float   f = (now - m_timeGrow) / c_dtGrow;
#else
                    float   f = (now - m_timeGrow).count() / c_dtGrow;
#endif

                    position = m_positionStop;
                    radius = m_radiusMax * f + m_radiusAsteroid * (1.0f - f);
                    aInner = 0.5f;
                    fInner = 1.0f;
                    fOuter = 1.0f;
                }
                else
                {
#ifdef WIN
                    float   dt = (now - m_timeEnvelope);
#else
                    float dt = (now-m_timeEnvelope).count();
#endif
                    float   f = (dt > 0.0f) ? (dt / c_dtEnvelope) : 0.0f;

                    position = m_positionStop * f + m_positionStart * (1.0f - f);
                    radius = f * m_radiusAsteroid + 1.0f - f;

                    aInner = 0.5f;
                    fInner = 1.0f;
                    fOuter = 1.0f;
                }

                SetPosition(position);
                SetRadius(radius);

                assert (GetThingSite());
                GetThingSite()->SetColors(aInner, fInner, fOuter);
            }
        }

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_buildingEffect;
        }

    // ImodelIGC
        virtual void    SetCluster(IclusterIGC* cluster)
        {
            AddRef();

            {
                IclusterIGC*    c = GetCluster();
                if (c)
                    c->DeleteModel(this);
            }

            TmodelIGC<IbuildingEffectIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddModel(this);

            Release();
        }
        virtual void                 HandleCollision(Time       timeCollision,
                                                     float                  tCollision,
                                                     const CollisionEntry&  entry,
                                                     ImodelIGC* pModel)
        {
            ObjectType  type = pModel->GetObjectType();
            if ((type != OT_buildingEffect) &&
                (type != OT_asteroid) &&
                (type != OT_station) &&
                (type != OT_probe))
            {
                pModel->HandleCollision(timeCollision, tCollision, entry, this);
            }
        }

    // IdamageIGC
        virtual DamageResult        ReceiveDamage(DamageTypeID            type,
                                                  float                   amount,
                                                  Time                    timeCollision,
                                                  const Vector&           position1,
                                                  const Vector&           position2,
                                                  ImodelIGC*              launcher)
        {
            if (m_pasteroid)
            {
                m_pasteroid->ReceiveDamage(type, amount, timeCollision, m_pasteroid->GetPosition(), position2, launcher);
            }

            return c_drNoDamage;
        }
        virtual float   GetFraction(void) const
        {
            return 1.0f;
        }
        virtual void    SetFraction(float newVal)
        {
        }

        virtual float   GetHitPoints(void) const
        {
            return m_pasteroid ? m_pasteroid->GetHitPoints() : 1.0f;
        }

    // IbuildingEffectIGC
        virtual void    BuilderTerminated(void)
        {
            m_pshipBuilder = NULL;
        }

        virtual IasteroidIGC*    GetAsteroid(void) const
        {
#ifdef WIN
            return m_pasteroid;
#else
            return m_pasteroid.get();
#endif
        }

        virtual void    MakeUnhitable(void)
        {
            assert (m_pasteroid);
            m_pasteroid = NULL;

            TmodelIGC<IbuildingEffectIGC>::MakeUnhitable();        //Virtual wrapper for a non-virtual function
        }

        virtual void                AsteroidUpdate(Time now)
        {
            if ((now >= m_timeShrink) && m_pshipBuilder)
            {
                //transform the asteroid into the station
                assert (m_pstationType);
                assert (m_pside);
#ifdef WIN
                GetMyMission()->GetIgcSite()->SendChatf(m_pshipBuilder, CHAT_TEAM, m_pside->GetObjectID(), m_pstationType->GetCompletionSound(), "Finished building %s", m_pstationType->GetName());
#else
                GetMyMission()->GetIgcSite()->SendChatf(m_pshipBuilder.get(), CHAT_TEAM, m_pside->GetObjectID(), m_pstationType->GetCompletionSound(), "Finished building %s", m_pstationType->GetName());
#endif

				//Imago #120 #121 8/10
				bool bseenside[c_cSidesMax] = {false};
				GetMyMission()->GetSeenSides(GetMyMission()->GetModel(m_pshipBuilder->GetObjectType(),m_pshipBuilder->GetObjectID()),bseenside,GetMyMission()->GetModel(m_pasteroid->GetObjectType(),m_pasteroid->GetObjectID()));
								
                //Quietly kill the ship (after nuking its parts to prevent treasure from being created)
                {
#ifdef WIN
                    const PartListIGC*  parts = m_pshipBuilder->GetParts();
                    PartLinkIGC*    plink;
                    while (plink = parts->first())  //Not ==
                        plink->data()->Terminate();
#else
                    const PartListIGC*  parts = m_pshipBuilder->GetParts();
                    for( auto part : *parts )
                    {
                      part->Terminate();
                    }
#endif
                }
                m_pshipBuilder->SetAmmo(0);
                m_pshipBuilder->SetFuel(0.0f);

                m_pshipBuilder->SetStateM(0);

#ifdef WIN
                GetMyMission()->GetIgcSite()->KillShipEvent(now, m_pshipBuilder, NULL, 0.0f, m_pshipBuilder->GetPosition(), Vector::GetZero());
#else
                GetMyMission()->GetIgcSite()->KillShipEvent(now, m_pshipBuilder.get(), NULL, 0.0f, m_pshipBuilder->GetPosition(), Vector::GetZero());
#endif
                m_pshipBuilder = NULL;

#ifdef WIN
                IasteroidIGC*   pasteroid = m_pasteroid;
#else
                IasteroidIGC*   pasteroid = m_pasteroid.get();
#endif

                m_pasteroid->SetBuildingEffect(NULL);     //Clear the building effect so it isn't nuked along with the asteroid
                assert (m_pasteroid == NULL);
#ifdef WIN
                GetMyMission()->GetIgcSite()->BuildStation(pasteroid, m_pside, m_pstationType, now, bseenside); //Imago #121 - Selective pop rocks
#else
                GetMyMission()->GetIgcSite()->BuildStation(pasteroid, m_pside.get(), m_pstationType.get(), now, bseenside); //Imago #121 - Selective pop rocks
#endif
            }
        }

    private:
        TRef<IshipIGC>           m_pshipBuilder;
        TRef<IasteroidIGC>       m_pasteroid;
        TRef<IstationTypeIGC>    m_pstationType;
        TRef<IsideIGC>           m_pside;
        Time                     m_timeComplete;
        Time                     m_timeShrink;
        Time                     m_timeOpaque;
        Time                     m_timeGrow;
        Time                     m_timeEnvelope;

        Vector                   m_positionStart;
        Vector                   m_positionStop;
        float                    m_radiusAsteroid;
        float                    m_radiusStation;
        float                    m_radiusMax;
};

#endif
