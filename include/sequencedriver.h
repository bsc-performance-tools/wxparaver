/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  wxparaver                                *
 *              Paraver Trace Visualization and Analysis Tool                *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

#pragma once


#include "traceeditactions.h"

class gTimeline;

/****************************************************************************
 ********             RunAppClusteringAction                         ********
 ****************************************************************************/
class RunAppClusteringAction: public TraceToTraceAction
{
  public:
    RunAppClusteringAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~RunAppClusteringAction()
    {}

    virtual std::vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********              RunAppCutterAction                            ********
 ****************************************************************************/
class RunAppCutterAction: public TraceToTraceAction
{
  public:
    RunAppCutterAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~RunAppCutterAction()
    {}

    virtual std::vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********              RunAppDimemasAction                           ********
 ****************************************************************************/
class RunAppDimemasAction: public TraceToTraceAction
{
  public:
    RunAppDimemasAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~RunAppDimemasAction()
    {}

    virtual std::vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********              RunAppFoldingAction                           ********
 ****************************************************************************/
class RunAppFoldingAction: public TraceToTraceAction
{
  public:
    RunAppFoldingAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~RunAppFoldingAction()
    {}

    virtual std::vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********              RunSpectralAction                             ********
 ****************************************************************************/
class RunSpectralAction: public TraceToTraceAction
{
  public:
    RunSpectralAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~RunSpectralAction()
    {}

    virtual std::vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********              RunAppUserCommandAction                       ********
 ****************************************************************************/
class RunAppUserCommandAction: public TraceToTraceAction
{
  public:
    RunAppUserCommandAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~RunAppUserCommandAction()
    {}

    virtual std::vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********              ExternalSortAction                            ********
 ****************************************************************************/
class ExternalSortAction: public TraceToTraceAction
{
  public:
    ExternalSortAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~ExternalSortAction()
    {}

    virtual std::vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********                 SequenceDriver                             ********
 ****************************************************************************/
class SequenceDriver
{
  public:
    static void sequenceClustering( gTimeline *whichTimeline );
    static void sequenceCutter( gTimeline *whichTimeline );
    static void sequenceDimemas( gTimeline *whichTimeline );
    static void sequenceFolding( gTimeline *whichTimeline );
    static void sequenceSpectral( gTimeline *whichTimeline );
    static void sequenceUserCommand( gTimeline *whichTimeline );

    //static void sequenceTraceShifter( std::string trace,
    //                                    std::string timesFile );
};


